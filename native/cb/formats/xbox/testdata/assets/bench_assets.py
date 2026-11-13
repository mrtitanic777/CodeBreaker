import sys, time, os
sys.path.insert(0, '.')
from codebreaker.formats.xbox import xdbf, xur, xbe, xpr, bink, chd
mc = 'state/work/xboxfmt/mc'
asset = 'native/cb/formats/xbox/testdata/assets'
spa = open(mc + '/spa0.xdbf', 'rb').read()
scenes = [open(mc + '/scene%02d.xur' % i, 'rb').read() for i in range(12)]
xbe_demo = open(asset + '/xbe_demo.bin', 'rb').read()
xpr0 = open(asset + '/xpr0.bin', 'rb').read()


def t(name, fn, n):
    s = time.perf_counter()
    for _ in range(n):
        fn()
    e = time.perf_counter() - s
    print('%-28s %8.3f ms/op' % (name, e / n * 1000))


def spa_op():
    x = xdbf.XDBF(spa)
    x.counts()
    x.achievements()
    x.build()


def xur_op():
    for d in scenes:
        x = xur.XUR(d)
        x.strings()


def xbe_op():
    x = xbe.XBE(xbe_demo)
    x.sections()
    xbe.pe_image(x)


def xpr_op():
    xpr.to_dds(xpr.parse(xpr0))


def hdr_op():
    bink.read_header(asset + '/t.bik')
    chd.read_header(asset + '/t.chd')


t('xdbf spa parse+ach+build', spa_op, 200)
t('xur 12 scenes strings', xur_op, 100)
t('xbe parse+sections+pe', xbe_op, 500)
t('xpr0 parse+dds', xpr_op, 500)
t('bink+chd header (files)', hdr_op, 500)
