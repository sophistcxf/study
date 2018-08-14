from datetime import datetime
from datetime import timedelta
def get_date_list(start, end):
    date_list = []
    begin_date = datetime.strptime(start, "%Y%m%d%H%M%S").date()
    print begin_date
    end_date = datetime.strptime(end, "%Y%m%d%H%M%S").date()
    cur_date = begin_date
    while cur_date <= end_date:
        date_list.append(cur_date.strftime("%Y%m%d%H%M%S"))
        cur_date = cur_date + timedelta(0,0,120)
        print cur_date
    return date_list

print get_date_list("20171001100000", "20171001101000")
