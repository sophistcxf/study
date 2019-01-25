import json
from collections import Counter

def test_load():
    data = '{"a":1,"b":2,"c":3,"d":4,"e":5, "f":[1, 2]}'
    data = '{"link_route":["link merge no_excep_speed(24.83,24.83) > 0"],"sections":[{"route":["S4","S0","S4","S0","S4","S0"],"si":0},{"route":["S4","S0","102 abnormal car few sample","S4","S0"],"si":1}]}'
    data = '{"link_route":["link merge no_excep_speed(25.55,25.55) > 0"],"sections":[{"route":["S3","S0","S3","S0"],"si":0}]}'
    text = json.loads(data)
    if 'sections' in text:
        for si in text['sections']:
            if 'route' in si:
                route = si['route']
                print route
                c = Counter(route)
                for i,v in c:
                    print i,v
        
    print text

if __name__ == "__main__":
    test_load()
