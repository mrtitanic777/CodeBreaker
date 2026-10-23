# codebreaker/formats/xbox/crypto

Xbox 360 crypto, keyvault, boot-chain and NAND tooling from **Xbox-360-Crypto** by GoobyCorp,
under the **BSD 3-Clause License** (see LICENSE). Copyright and license are retained per its terms.

From upstream, kept as written (imports made relative to this package; the folders they read
named by settings): `XeCrypt.py`, `MemCrypto.py`, `bin2lang.py`, `build_lib.py`, `cpu_key.py`,
`create_key.py`, `exp_build_task.py`, `exp_signer.py`, `export_keys.py`, `format_key.py`,
`harness.py`, `kdnet.py`, `keystore.py`, `kv_tool.py`, `lzx.py`, `nand_tool.py`,
`patch_build_task.py`, `patch_checker.py`, `patch_compile.py`, `patch_dumper.py`, `sd_signer.py`,
`se_patcher.py`, `shadowboot.py`, `xcp_dumper.py`, `xdk_extract.py`, `xebuild_config.py`,
`xekeys_signer.py`, `xell_unpack.py`, `xkelib_exports.py`, `xsm3.py`, `xval.py`.
Changes to upstream: `XeCrypt.py` takes ARC4 and TripleDES from where cryptography 43 moved them;
`shadowboot.py` loads its keys when it runs rather than on import; `xkelib_exports.py` finds the
SDK through the `xbox360_sdk` setting.

CodeBreaker's own, built on it: `bootchain.py`, `cpukey_ecd.py`, `flashplan.py`, `forensics.py`,
`fuses.py`, `hypervisor.py`, `keyvault.py`, `nand.py`, `smc.py`, `StreamIO.py` (the stream class the
upstream tools read images through; upstream's own module was not carried over) and `__init__.py`
(the `crypto` command).

Not carried over: upstream's `bin/` folder (Microsoft's LZX libraries, the Free60 binutils, captured
request blobs -- point the `xbox360_crypto_bin` setting at a copy), `kv_checker.py` (needs those
blobs and checks a keyvault against Xbox LIVE), and `assembler.py` (needs keystone; CodeBreaker
has its own PowerPC tools).
