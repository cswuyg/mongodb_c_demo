#!/home/work/bin/python
#test find  with tag_set
#cswuyg @ 2014.7.18
# install pymongo pre.

import pymongo
import time

REMOTE_ADDRESS = "xxxxhost"
REMOTE_PORT = 27030
def _test_find():
    f_w = open("test", 'w')
    #client = pymongo.MongoClient(REMOTE_ADDRESS, 27030, readPreference='secondaryPreferred')
    rpre = pymongo.read_preferences.Secondary(tag_sets = [{'location': 'gz'}])
    client = pymongo.MongoClient(REMOTE_ADDRESS, REMOTE_PORT, read_preference=rpre) # 注意这里的read_preference
    s = time.time()
    docs = client.myapp.myuser.find({'name': 'cswuyg'})
    with open('tmp.txt', 'w') as f_w: 
        for item in docs:
            f_w.write(str(item))
            f_w.write('\n')

    e = time.time()
    print(e-s)

if __name__ == "__main__":
    _test_find()



