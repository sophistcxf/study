import configparser

parser = configparser.ConfigParser()
parser.read('config.ini')

print(parser.sections())

print(parser.get('person', 'name'))

try:
    print(parser.get('person', 'invalid_key'))
except configparser.NoOptionError as ex:
    print(ex)

print(parser.get('person', 'invalid_key', fallback=None))
