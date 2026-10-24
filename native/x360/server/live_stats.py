"""live_stats.py -- the LIVE server's leaderboards (network_protocol.md "Stats"): a title's stats views, one row
per player, ranked by the rating column (65534) highest first, ties by who got there first. Writes apply each
column's aggregation as Xbox LIVE did: last (default), sum, min, max. Kept in the data file. A mixin of
live_server.Server.
"""
import time

RATING = 65534
AGGREGATIONS = ("last", "sum", "min", "max")
TYPES = (1, 2, 3, 5, 7)            # XUSER_DATA_TYPE: int32, int64, double, float, date/time (FILETIME)
VIEWS_MAX, COLUMNS_MAX, XUIDS_MAX, ROWS_MAX = 10, 65, 101, 100


def _number(v, typ):
    return float(v) if typ in (3, 5) else int(v)


class StatsOps:
    def _board(self, title_id, view):
        return self.store.stats.setdefault(str(title_id).upper(), {}).setdefault(str(int(view)), {})

    def _ranked(self, board):
        """[(xuid, row)] best first: the rating column's value, highest first; ties by who reached it first."""
        def key(item):
            col = item[1]["columns"].get(str(RATING))
            return (-(col["value"] if col else 0), item[1].get("at", 0))
        return sorted(board.items(), key=key)

    def _row(self, rank, xuid, row, wanted):
        rating = row["columns"].get(str(RATING), {}).get("value", 0)
        cols = [dict(row["columns"][str(c)]) for c in wanted if str(c) in row["columns"]]
        return {"xuid": xuid, "gamertag": self.store.accounts.get(xuid, ""), "rank": rank, "rating": rating,
                "columns": cols}

    def _views(self, msg):
        views = msg.get("views")
        if not isinstance(views, list) or not 1 <= len(views) <= VIEWS_MAX:
            raise ValueError("bad views")
        return views

    def op_stats_write(self, conn, msg):
        """A player's columns for one or more views (a host writes for remote players too)."""
        self.session(conn, msg)
        title_id = str(msg.get("title_id", "")).upper()
        xuid = str(msg.get("xuid", "")).upper()
        if not title_id or xuid not in self.store.accounts:
            raise ValueError("bad title or xuid")
        now = time.time()
        for v in self._views(msg):
            columns = v.get("columns", [])
            if not isinstance(columns, list) or len(columns) > COLUMNS_MAX:
                raise ValueError("bad columns")
            board = self._board(title_id, v["view"])
            row = board.setdefault(xuid, {"columns": {}, "at": now})
            rating_before = row["columns"].get(str(RATING), {}).get("value")
            for c in columns:
                cid, typ = int(c["id"]), int(c.get("type", 1))
                if typ not in TYPES:
                    raise ValueError("bad column type")
                agg = str(c.get("aggregation", "last"))
                if agg not in AGGREGATIONS:
                    raise ValueError("bad aggregation")
                value = _number(c["value"], typ)
                old = row["columns"].get(str(cid))
                if old is not None and agg != "last":
                    value = {"sum": old["value"] + value, "min": min(old["value"], value),
                             "max": max(old["value"], value)}[agg]
                row["columns"][str(cid)] = {"id": cid, "type": typ, "value": value}
            if row["columns"].get(str(RATING), {}).get("value") != rating_before:
                row["at"] = now                                     # ties: who got there first
        self.store.save()

    def op_stats_read(self, conn, msg):
        self.session(conn, msg)
        title_id = str(msg.get("title_id", "")).upper()
        xuids = [str(x).upper() for x in msg.get("xuids", [])]
        if not 1 <= len(xuids) <= XUIDS_MAX:
            raise ValueError("bad xuids")
        out = []
        for v in self._views(msg):
            board = self._board(title_id, v["view"])
            ranks = {x: i + 1 for i, (x, _r) in enumerate(self._ranked(board))}
            rows = [self._row(ranks[x], x, board[x], v.get("columns", [])) if x in board else
                    {"xuid": x, "gamertag": self.store.accounts.get(x, ""), "rank": 0, "rating": 0, "columns": []}
                    for x in xuids]
            out.append({"view": v["view"], "total": len(board), "rows": rows})
        return {"views": out}

    def op_stats_enumerate(self, conn, msg):
        """Rows in rank order from a rank, around a player, or from the first at or below a rating."""
        self.session(conn, msg)
        title_id = str(msg.get("title_id", "")).upper()
        by = str(msg.get("by", "rank"))
        count = max(1, min(ROWS_MAX, int(msg.get("rows", 10))))
        out = []
        for v in self._views(msg):
            ranked = self._ranked(self._board(title_id, v["view"]))
            if by == "rank":
                start = max(1, int(msg.get("rank", 1))) - 1
            elif by == "xuid":
                who = str(msg.get("xuid", "")).upper()
                at = next((i for i, (x, _r) in enumerate(ranked) if x == who), None)
                start = None if at is None else max(0, at - count // 2)
            elif by == "rating":
                rating = float(msg.get("rating", 0))
                start = next((i for i, (_x, r) in enumerate(ranked)
                              if r["columns"].get(str(RATING), {}).get("value", 0) <= rating), len(ranked))
            else:
                raise ValueError("bad by")
            rows = [] if start is None else [self._row(i + 1, x, r, v.get("columns", []))
                                             for i, (x, r) in enumerate(ranked) if start <= i < start + count]
            out.append({"view": v["view"], "total": len(ranked), "rows": rows})
        return {"views": out}

    def op_stats_reset(self, conn, msg):
        """XUserResetStatsView: the user's rows leave one view (`view`) or all of the title's (0)."""
        s = self.session(conn, msg)
        title_id = str(msg.get("title_id", "")).upper()
        view = int(msg.get("view", 0))
        boards = self.store.stats.get(title_id, {})
        for v, board in boards.items():
            if view == 0 or v == str(view):
                board.pop(s.xuid, None)
        self.store.save()
