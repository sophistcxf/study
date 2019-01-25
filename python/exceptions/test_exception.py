def test_exception():
    try:
        #print [1,2,3][10]
        print {'a':1, 'b':2}['c']
    except IndexError as e:
        print "IndexError ", e
    except KeyError as e:
        print "KeyError ", e
    except BaseException as e:
        print "BaseException ", e

if __name__ == "__main__":
    test_exception()
