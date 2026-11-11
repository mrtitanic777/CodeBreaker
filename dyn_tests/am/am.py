
import asyncio

async def leaf(x):
    return x * 2

async def mid(x):
    a = await leaf(x)
    return a + await leaf(a)

async def agen(n):
    for i in range(n):
        await asyncio.sleep(0)
        yield i

async def collect():
    out = []
    async for v in agen(3):
        if v == 5:
            break
        out.append(v)
    else:
        out.append('done')
    return out + [w async for w in agen(2) if w]

class Lock:
    async def __aenter__(self):
        return self
    async def __aexit__(self, *exc):
        return False

async def guarded(lock):
    async with lock as held, Lock():
        return await mid(1)

def plain():
    async def inner():
        return 1
    return inner
