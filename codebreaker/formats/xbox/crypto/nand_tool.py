#!/usr/bin/env python3

from io import BytesIO
from pathlib import Path
from struct import pack, unpack_from
from ctypes import LittleEndianStructure, c_ubyte, c_uint16, c_uint32, c_uint64

# constants
SEEK_SET = 0
SEEK_CUR = 1
SEEK_END = 2

MODE_FILE = 0
MODE_FLASH = 1

# types
BYTE  = c_ubyte
WORD  = c_uint16
DWORD = c_uint32
QWORD = c_uint64

class SMALLBLOCK(LittleEndianStructure):
	def __getattribute__(self, item):
		if item == "block_id":
			res = ((self.block_id_0 << 8) & 0xF00) + (self.block_id_1 & 0xFF)
		elif item == "fs_size":
			res = ((self.fs_size_0 << 8) & 0xFF00) + (self.fs_size_1 & 0xFF)
		else:
			res = super(SMALLBLOCK, self).__getattribute__(item)
		return res

	_fields_ = [
		("block_id_1", BYTE),  # lba/id = (((BlockID0<<8)&0xF)+(BlockID1&0xFF))
		("block_id_0", BYTE, 4),
		("fs_unused_0", BYTE, 4),
		("fs_sequence_0", BYTE),
		("fs_sequence_1", BYTE),
		("fs_sequence_2", BYTE),
		("bad_block", BYTE),
		("fs_sequence_3", BYTE),
		("fs_size_1", BYTE),  # (((FsSize0<<8)&0xFF)+(FsSize1&0xFF)) = cert size
		("fs_size_0", BYTE),
		("fs_page_count", BYTE),
		# free pages left in block (ie: if 3 pages are used by cert then this would be 29:0x1d)
		("fs_unused_1", BYTE * 2),
		("fs_block_type", BYTE, 6),
		("ecc_3", BYTE, 2),  # 26 bit ECD
		("ecc_2", BYTE),
		("ecc_1", BYTE),
		("ecc_0", BYTE)
	]


class BIGONSMALL(LittleEndianStructure):
	def __getattribute__(self, item):
		if item == "block_id":
			res = ((self.block_id_0 << 8) & 0xF00) + (self.block_id_1 & 0xFF)
		elif item == "fs_size":
			res = ((self.fs_size_0 << 8) & 0xFF00) + (self.fs_size_1 & 0xFF)
		else:
			res = super(BIGONSMALL, self).__getattribute__(item)
		return res

	_fields_ = [
		("fs_sequence_0", BYTE),
		("block_id_1", BYTE),  # lba/id = (((BlockID0<<8)&0xF)+(BlockID1&0xFF))
		("block_id_0", BYTE, 4),
		("fs_unused_0", BYTE, 4),
		("fs_sequence_1", BYTE),
		("fs_sequence_2", BYTE),
		("bad_block", BYTE),
		("fs_sequence_3", BYTE),
		("fs_size_1", BYTE),  # (((FsSize0<<8)&0xFF)+(FsSize1&0xFF)) = cert size
		("fs_size_0", BYTE),
		("fs_page_count", BYTE),
		# free pages left in block (ie: if 3 pages are used by cert then this would be 29:0x1d)
		("fs_unused_1", BYTE * 2),
		("fs_block_type", BYTE, 6),
		("ecc_3", BYTE, 2),  # 26 bit ECD
		("ecc_2", BYTE),
		("ecc_1", BYTE),
		("ecc_0", BYTE)
	]


class BIGBLOCK(LittleEndianStructure):
	def __getattribute__(self, item):
		if item == "block_id":
			res = ((self.block_id_0 << 8) & 0xF00) + (self.block_id_1 & 0xFF)
		elif item == "fs_size":
			res = ((self.fs_size_0 << 8) & 0xFF00) + (self.fs_size_1 & 0xFF)
		else:
			res = super(BIGBLOCK, self).__getattribute__(item)
		return res

	def __setattr__(self, key, value):
		if key == "block_id":
			raise NotImplementedError("block_id can't be set yet!")
		elif key == "fs_size":
			raise NotImplementedError("fs_size can't be set yet!")
		else:
			res = super(BIGBLOCK, self).__setattr__(key, value)
		return res

	_fields_ = [
		("bad_block", BYTE),
		("block_id_1", BYTE),  # lba/id = (((BlockID0<<8)&0xF)+(BlockID1&0xFF))
		("block_id_0", BYTE, 4),
		("fs_unused_0", BYTE, 4),
		("fs_sequence_1", BYTE),
		("fs_sequence_2", BYTE),
		("fs_sequence_0", BYTE),
		("fs_sequence_3", BYTE),
		("fs_size_1", BYTE),
		# FS: 06 (system reserve block number) else (((FsSize0<<8)&0xFF)+(FsSize1&0xFF)) = cert size
		("fs_size_0", BYTE),  # FS: 20 (size of flash filesys in smallblocks >>5)
		("fs_page_count", BYTE),
		# FS: 04 (system config reserve) free pages left in block (multiples of 4 pages, ie if 3f then 3f*4 pages are free after)
		("fs_unused_1", BYTE * 2),
		("fs_block_type", BYTE, 6),
		("ecc_3", BYTE, 2),  # 26 bit ECD
		("ecc_2", BYTE),
		("ecc_1", BYTE),
		("ecc_0", BYTE)
	]

def calc_page_ecc(data: bytes | bytearray, spare: bytes | bytearray) -> int:
	if type(data) == bytes:
		data = bytearray(data)

	val = 0
	v = 0
	idx = 0
	for bit in range(0x1066):
		if not (bit & 31):
			if bit == 0x1000:
				data = spare
				idx = 0
			(v,) = unpack_from("<I", data, idx)
			v = ~v
			idx += 4
		val ^= v & 1
		v >>= 1
		if val & 1:
			val ^= 0x6954559
		val >>= 1
	return ~val & 0xFFFFFFFF

def fix_page_ecc(data: bytes | bytearray, spare: bytes | bytearray) -> tuple:
	if type(spare) == bytes:
		spare = bytearray(spare)

	val = calc_page_ecc(data, spare)
	spare[12] = (spare[12] & 0x3F) + ((val << 6) & 0xC0)
	spare[13] = (val >> 2) & 0xFF
	spare[14] = (val >> 10) & 0xFF
	spare[15] = (val >> 18) & 0xFF
	return (data, spare)

class NANDImage:
	mode = MODE_FILE
	stream = None
	file_size = 0
	flash_size = 0
	num_pages = 0

	@property
	def file_offset(self) -> int:
		return self._file_offset

	@file_offset.setter
	def file_offset(self, offset: int) -> None:
		self._flash_offset = self.file_to_flash_offset(offset)
		self._file_offset = offset
		self.stream.seek(self._file_offset)

	@property
	def flash_offset(self) -> int:
		return self._flash_offset

	@flash_offset.setter
	def flash_offset(self, offset: int) -> None:
		self._file_offset = self.flash_to_file_offset(offset)
		self._flash_offset = offset
		self.stream.seek(self._file_offset)

	def __init__(self, filename_or_data: (str, bytes, bytearray), mode: int = MODE_FILE):
		self.reset()

		self.mode = mode
		if type(filename_or_data) == str:
			self.stream = open(filename_or_data, "r+b")
		elif type(filename_or_data) in [bytes, bytearray]:
			self.stream = BytesIO(filename_or_data)

		# seek to the end
		self.file_seek(0, SEEK_END)
		# get size with spare data
		self.file_size = self.file_tell()
		# get number of pages
		self.num_pages = self.file_size // 528
		# get size without spare data
		self.flash_size = self.num_pages * 512
		# seek back to the start
		self.file_seek(0)

	def __enter__(self):
		return self

	def __exit__(self, exc_type, exc_val, exc_tb) -> None:
		self.stream.flush()
		self.stream.close()

	def reset(self) -> None:
		self.mode = MODE_FILE
		self.stream = None
		self.file_size = 0
		self.flash_size = 0
		self.num_pages = 0

	# stream primitives
	def seek(self, offset: int, whence: int = SEEK_SET) -> int:
		return getattr(self, ("file" if self.mode == MODE_FILE else "flash") + "_seek")(offset, whence)

	def tell(self) -> int:
		return getattr(self, ("file" if self.mode == MODE_FILE else "flash") + "_tell")()

	def read(self, num: int = 0) -> bytes:
		return getattr(self, ("file" if self.mode == MODE_FILE else "flash") + "_read")(num)

	def write(self, data: (bytes, bytearray)) -> int:
		return getattr(self, ("file" if self.mode == MODE_FILE else "flash") + "_write")(data)

	def flush(self) -> None:
		self.stream.flush()

	# extended I/O functions

	# offset translation functions
	def file_to_flash_offset(self, file_offset: int) -> int:
		"""
		Convert file offset to flash offset
		:param flash_offset:
		:return:
		"""
		if file_offset < 0:
			file_offset = self.flash_size + file_offset
		return ((file_offset // 528) * 512) + (file_offset % 528)

	def flash_to_file_offset(self, flash_offset: int) -> int:
		"""
		Convert flash offset to file offset
		:param file_offset:
		:return:
		"""
		if flash_offset < 0:
			flash_offset = self.file_size + flash_offset
		return ((flash_offset // 512) * 528) + (flash_offset % 512)

	def file_offset_to_page(self, offset: int) -> int:
		"""
		Get the page a file offset lands on
		:param offset:
		:return:
		"""
		return (offset // 528) + 1

	def flash_offset_to_page(self, offset: int) -> int:
		"""
		Get the page a flash offset lands on
		:param offset:
		:return:
		"""
		return (offset // 512) + 1

	def flash_calc_page_offset(self, offset: int) -> int:
		"""
		Calculates the start or end offset for page I/O
		:param offset:
		:return:
		"""
		return offset - ((offset // 512) * 512)

	def calc_page_offset(self, num: int) -> int:
		"""
		Calculates the start offset for a given page number
		:param num: The page number
		:return:
		"""
		return (num - 1) * 528

	def calc_spare_offset(self, num: int) -> int:
		so = ((num - 1) * 528) - 16
		return 512 if (num - 1) == 0 else so

	# file primitives
	def file_tell(self) -> int:
		return self.stream.tell()

	def file_seek(self, offset: int, whence: int = SEEK_SET) -> int:
		no = self.stream.seek(offset, whence)
		self.file_offset = no
		return no

	def file_read(self, num: int = 0) -> bytes:
		if num > 0:
			data = self.stream.read(num)
		else:
			data = self.stream.read()
		self.file_offset = self.file_tell()
		return data

	def file_write(self, data: (bytes, bytearray)) -> int:
		nw = self.stream.write(data)
		self.file_offset = self.file_tell()
		return nw

	# flash primitives
	def flash_tell(self) -> int:
		return self.file_to_flash_offset(self.file_tell())

	def flash_seek(self, offset: int, whence: int = SEEK_SET) -> int:
		no = 0  # promise
		if whence == SEEK_SET:
			if offset >= 0:
				# no = self.file_seek(self.flash_to_file_offset(offset), SEEK_SET)
				self.flash_offset = offset
				no = self.flash_tell()
			elif offset < 0:
				no = self.file_seek(self.flash_to_file_offset(self.flash_size - offset), SEEK_SET)
		elif whence == SEEK_CUR:
			if offset >= 0:
				# no = self.file_seek(self.flash_to_file_offset(offset), SEEK_CUR)
				self.flash_offset += offset
				no = self.flash_tell()
			elif offset < 0:
				no = self.file_seek(self.flash_to_file_offset(self.flash_offset - offset), SEEK_CUR)
		elif whence == SEEK_END:
			if offset == 0:
				no = self.file_seek(0, SEEK_END)
			elif offset < 0:
				no = self.file_seek(self.flash_to_file_offset(self.flash_size - offset), SEEK_END)
		# self.file_offset = no
		return self.file_to_flash_offset(no)

	def flash_read(self, num: int = 0) -> bytes:
		strt_page = self.flash_offset_to_page(self.flash_offset)
		strt_offs = self.flash_calc_page_offset(self.flash_offset)
		stop_page = self.flash_offset_to_page(self.flash_offset + num)
		stop_offs = self.flash_calc_page_offset(self.flash_offset + num)

		#print("\nFlash Read:")
		#print(f"\tFlash Size:   0x{self.flash_size:04X}")
		#print(f"\tStart Page:   {strt_page}")
		#print(f"\tStart Offset: {strt_offs}")
		#print(f"\tStop Page:    {stop_page}")
		#print(f"\tStop Offset:  {stop_offs}")

		# page by page, the first and last possibly partial
		data = bytearray()
		offset = self.flash_offset
		while len(data) < num:
			in_page = offset % 512
			take = min(512 - in_page, num - len(data))
			data += self.get_page(offset // 512 + 1)[in_page:in_page + take]
			offset += take
		# self.flash_offset = self.flash_tell()
		return bytes(data)

	def flash_write(self, data: bytes | bytearray) -> int:
		strt_page = self.flash_offset_to_page(self.flash_offset)
		strt_offs = self.flash_calc_page_offset(self.flash_offset)
		stop_page = self.flash_offset_to_page(self.flash_offset + len(data))
		stop_offs = self.flash_calc_page_offset(self.flash_offset + len(data))

		# print("\nFlash Write:")
		# print(f"\tFlash Size:   0x{self.flash_size:04X}")
		# print(f"\tStart Page:   {strt_page}")
		# print(f"\tStart Offset: {strt_offs}")
		# print(f"\tStop Page:    {stop_page}")
		# print(f"\tStop Offset:  {stop_offs}")

		nw = 0
		# page by page (each page's ECC is recomputed by set_page), the first and last possibly partial
		offset = self.flash_offset
		done = 0
		while done < len(data):
			in_page = offset % 512
			take = min(512 - in_page, len(data) - done)
			tmp_page = bytearray(self.get_page(offset // 512 + 1))
			tmp_page[in_page:in_page + take] = data[done:done + take]
			nw += self.set_page(offset // 512 + 1, tmp_page)
			offset += take
			done += take
		# self.flash_offset = self.flash_tell()
		return nw

	# page I/O
	def get_page(self, num: int) -> bytes:
		"""
		Get a page by page number
		:param num:
		:return:
		"""
		self.file_seek(self.calc_page_offset(num))
		return self.file_read(512)

	def get_spare(self, num: int) -> bytes:
		"""
		Get a spare by page number
		:param num:
		:return:
		"""
		self.file_seek(self.calc_spare_offset(num))
		return self.file_read(16)

	def set_page(self, num: int, data: bytes | bytearray) -> int:
		"""
		Set a page by page number
		:param num:
		:param data:
		:return:
		"""
		assert 1 <= num <= self.num_pages, "Page number out of range"
		assert len(data) == 512, "Invalid page size"
		spare_data = self.get_spare(num)
		self.file_seek(self.calc_page_offset(num))
		(page_data, spare_data) = fix_page_ecc(data, spare_data)
		nw = self.file_write(page_data)
		nw += self.file_write(spare_data)
		return nw

	def set_spare(self, num: int, data: bytes | bytearray) -> int:
		"""
		Set a spare by spare number
		:param num:
		:param data:
		:return:
		"""
		assert 1 <= num <= self.num_pages, "Page number out of range"
		assert len(data) == 16, "Invalid spare size"
		self.file_seek(self.calc_spare_offset(num))
		return self.file_write(data)

# ---- image build / ECC batch operations (nandpro / xeBuild class) ----------
PAGE = 512
SPARE = 16
FULL = 528


def _stored_ecc(spare) -> int:
	"""The 26-bit ECC value packed into spare[12..15] by fix_page_ecc."""
	return ((spare[12] >> 6) & 3) | (spare[13] << 2) | (spare[14] << 10) | (spare[15] << 18)


def strip_spare(img: bytes) -> bytes:
	"""528-byte-page flash image  ->  raw 512-byte-page data (drop spare/meta)."""
	out = bytearray()
	for o in range(0, len(img) - FULL + 1, FULL):
		out += img[o:o + PAGE]
	return bytes(out)


def add_spare(raw: bytes, meta: bytes = None) -> bytes:
	"""raw 512-byte-page data  ->  528-byte-page image with correct ECC. `meta`
	(optional) supplies the per-page 16-byte spare template; ECC is recomputed."""
	out = bytearray()
	npages = len(raw) // PAGE
	for i in range(npages):
		page = raw[i * PAGE:(i + 1) * PAGE]
		spare = bytearray(meta[i * SPARE:i * SPARE + SPARE]) if meta else bytearray(SPARE)
		_, spare = fix_page_ecc(page, spare)
		out += page + bytes(spare)
	return bytes(out)


def verify_ecc(img: bytes) -> list:
	"""Return the list of page indices whose stored ECC does not match."""
	bad = []
	for i, o in enumerate(range(0, len(img) - FULL + 1, FULL)):
		page = img[o:o + PAGE]
		spare = img[o + PAGE:o + FULL]
		# only the low 26 bits of the computed value are stored in the spare
		if (calc_page_ecc(page, spare) & 0x3FFFFFF) != _stored_ecc(spare):
			bad.append(i)
	return bad


def fix_all_ecc(img: bytes) -> bytes:
	"""Recompute every page's ECC in place (repair after editing raw data)."""
	out = bytearray(img)
	for o in range(0, len(out) - FULL + 1, FULL):
		page = out[o:o + PAGE]
		spare = bytearray(out[o + PAGE:o + FULL])
		_, spare = fix_page_ecc(page, spare)
		out[o + PAGE:o + FULL] = spare
	return bytes(out)


DESCRIPTION = ("Xbox 360 NAND image build: ecc / unecc / verify / fix ECC "
               "(528-byte page + spare, nandpro/xeBuild class)")


def main() -> None:
	import sys
	a = sys.argv[1:]
	ops = ("info", "verify", "unecc", "ecc", "fix")
	if not a or a[0] in ("-h", "--help") or a[0] not in ops:
		print("usage:\n"
		      "  nandtool info   <img>              page/geometry summary\n"
		      "  nandtool verify <img>              check every page's ECC\n"
		      "  nandtool unecc  <img> <out>        strip spare -> raw 512-page data\n"
		      "  nandtool ecc    <raw> <out>        add spare + ECC -> flash image\n"
		      "  nandtool fix    <img> <out>        recompute all ECC in place")
		return
	op = a[0]
	data = open(a[1], "rb").read()
	if op == "info":
		full = len(data) // FULL
		print("file size   : 0x%X (%d bytes)" % (len(data), len(data)))
		if len(data) % FULL == 0:
			print("layout      : %d pages x 528 (512 data + 16 spare)" % full)
			print("flash size  : 0x%X (%d MB)" % (full * PAGE, full * PAGE // (1024 * 1024)))
			bad = verify_ecc(data)
			print("ECC         : %s" % ("all %d pages OK" % full if not bad
			                            else "%d bad pages (e.g. %s)" % (len(bad), bad[:8])))
		elif len(data) % PAGE == 0:
			print("layout      : %d raw pages x 512 (no spare) — use `ecc` to build" % (len(data) // PAGE))
		else:
			print("layout      : not a whole number of 512/528 pages")
		return
	if op == "verify":
		bad = verify_ecc(data)
		print("pages       : %d" % (len(data) // FULL))
		print("ECC result  : %s" % ("all pages valid" if not bad
		                            else "%d bad: %s%s" % (len(bad), bad[:16],
		                                                   " ..." if len(bad) > 16 else "")))
		sys.exit(1 if bad else 0)
	out = a[2] if len(a) > 2 else None
	if not out:
		print("usage: nandtool %s <in> <out>" % op); sys.exit(2)
	if op == "unecc":
		open(out, "wb").write(strip_spare(data))
		print("stripped %d pages -> %s (%d bytes raw)" % (len(data) // FULL, out, (len(data) // FULL) * PAGE))
	elif op == "ecc":
		res = add_spare(data)
		open(out, "wb").write(res)
		print("built %d pages with ECC -> %s (%d bytes)" % (len(data) // PAGE, out, len(res)))
	elif op == "fix":
		res = fix_all_ecc(data)
		open(out, "wb").write(res)
		print("recomputed ECC for %d pages -> %s" % (len(data) // FULL, out))


if __name__ == "__main__":
	main()