import imaplib
import sys

conn = imaplib.IMAP4("imap.qq.com",143)
username = sys.argv[1]
password = sys.argv[2]
conn.login(username, password)
conn.list()
conn.select()
type, data = conn.search(None, '(SUBJECT "Essh")')
msg=email.message_from_string(data[0][1])
content=msg.get_payload(decode=True)
print content
