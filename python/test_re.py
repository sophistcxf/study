import re

cxxxxf_pat = re.compile(".*cxxxxf.*")
print cxxxxf_pat.match("i'm cxxxxf")
print cxxxxf_pat.match("i'm cxxxf")

pa = re.compile("h.llo")
print pa.match("hllo")
print pa.match("hello")
print pa.match("heello")

pa = re.compile("h.+llo")
print pa.match("hello")
print pa.match("hllo")

# * indicates x occurs 0 to positive infinite times
pa = re.compile("cx*f")
print pa.match("cxxxxxxxxxf")
print pa.match("cxf")
print pa.match("cf")

# + indicates x occurs 1 to positive infinite times
pa = re.compile("cx+f")
print pa.match("cxxxxxxxxxf")
print pa.match("cxf")
print pa.match("cf")

ip_pattern = re.compile("")
