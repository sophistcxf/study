import json

content = open('sample.json', 'r').read()
j = json.loads(content)

def traverse_json(j_obj):
    for k, v in j_obj.items():
        print(k)
        if isinstance(v, dict):
            traverse_json(v)
        elif isinstance(v, list):
            for item in v:
                assert(not isinstance(item, list))
                if isinstance(item, dict):
                    traverse_json(item)
                else:
                    print(item)
        else:
            print(v)


traverse_json(j)
