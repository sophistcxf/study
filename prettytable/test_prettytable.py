import prettytable as pt

table = pt.PrettyTable(['row_idx', 'col1', 'col2', 'col3', 'col4'])

table.add_row(['row1', 'r1c1', 'r1c2', 'r1c3', 'r1c4'])
table.add_row(['row2', 'r2c1', 'r2c2', 'r2c3', 'r2c4'])

print table
