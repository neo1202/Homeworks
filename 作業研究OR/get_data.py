import requests
import  json
import pandas as pd
from pandas import json_normalize
from datetime import datetime
import time

def get_data():

    # using requests to "get" a webpage
    url = 'https://tcgbusfs.blob.core.windows.net/dotapp/youbike/v2/youbike_immediate.json'
    r = requests.get(url)
    print(r.status_code) # 200
    if r.status_code == requests.codes.ok: # OK!
        print("OK!")

    # get text on webpage
    r = requests.get(url)
    text = r.text
    info = json.loads(text)

    # Results contain the required data
    df = json_normalize(info) 
    print(df)
    filename = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    print(filename)
    df.to_csv(str(filename) + ".csv")
    print(filename + "finish")


# 調整間隔時間 單位（秒）
def Timer_catch():
    while True:
        time.sleep(60*60)
        get_data()
        Timer_catch()

Timer_catch()