type Point = tuple[int, int]
type PointList = list[Point]
type OffsetTable = tuple[PointList, PointList, PointList, PointList]

JLSTZ_OFFSET_TABLE: OffsetTable = (
    [( 0,  0), ( 0,  0), ( 0,  0), ( 0,  0), ( 0,  0)],
    [( 0,  0), (+1,  0), (+1, -1), ( 0, +2), (+1, +2)],
    [( 0,  0), ( 0,  0), ( 0,  0), ( 0,  0), ( 0,  0)],
    [( 0,  0), (-1,  0), (-1, -1), ( 0, +2), (-1, +2)],
)

I_OFFSET_TABLE: OffsetTable = (
    [( 0,  0), (-1,  0), (+2,  0), (-1,  0), (+2,  0)],
    [(-1,  0), ( 0,  0), ( 0,  0), ( 0, +1), ( 0, -2)],
    [(-1, +1), (+1, +1), (-2, +1), (+1,  0), (-2,  0)],
    [( 0, +1), ( 0, +1), ( 0, +1), ( 0, -1), ( 0, +2)],
)

O_OFFSET_TABLE: OffsetTable = (
    [( 0,  0)],
    [( 0, -1)],
    [(-1, -1)],
    [(-1,  0)],
)

def generate_offset_tables(name: str, table: OffsetTable):
    x_table = open(f"data/generated/{name}_x.bin", "wb")
    y_table = open(f"data/generated/{name}_y.bin", "wb")
    for rotation in table:
        rotation += [rotation[-1]] * (8 - len(rotation))
        for point in rotation:
            x_table.write(point[0].to_bytes(1, signed=True))
            y_table.write(point[1].to_bytes(1, signed=True))
    x_table.close()
    y_table.close()

generate_offset_tables("jlstz_offset_table", JLSTZ_OFFSET_TABLE)
generate_offset_tables("i_offset_table", I_OFFSET_TABLE)
generate_offset_tables("o_offset_table", O_OFFSET_TABLE)
