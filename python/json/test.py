#! coding=utf-8

import json
from collections import Counter

def test_load():
    data = '{"a":1,"b":2,"c":3,"d":4,"e":5, "f":[1, 2]}'
    data = '{"link_route":["link merge no_excep_speed(24.83,24.83) > 0"],"sections":[{"route":["S4","S0","S4","S0","S4","S0"],"si":0},{"route":["S4","S0","102 abnormal car few sample","S4","S0"],"si":1}]}'
    data = '{"link_route":["link merge no_excep_speed(25.55,25.55) > 0"],"sections":[{"route":["S3","S0","S3","S0"],"si":0}]}'
    text = json.loads(data)
    # text 是个字典
    print(type(text))
    if 'sections' in text:
        for si in text['sections']:
            if 'route' in si:
                route = si['route']
                print route
                c = Counter(route)
                for i,v in c:
                    print i,v
        
    print text

def test_parse_array(arr):
    json_arr = json.loads(arr)
    # json_arr 是个 list
    print(type(json_arr))
    for i in json_arr:
        print i

def test_must_be_double_quotation():
    '''
    json 串中的只能用双引号，不能用单引号
    '''
    data = '{"key":"value"}'
    j = json.loads(data)
    print(j)

    data = "{'key':'value'}"
    j = json.loads(data)
    print(j)

if __name__ == "__main__":
    #test_load()
    #test_parse_array('[1,2,3]')
    #test_parse_array('[{"a":1,"b":3},{"a":2},{"a":3}]')
    test_must_be_double_quotation()
