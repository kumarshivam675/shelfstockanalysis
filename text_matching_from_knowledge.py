#!/usr/bin/python
# -*- coding: latin-1 -*-
from nltk.corpus import wordnet
import csv
import centroid
from os import listdir
from os.path import isfile, join

import editdistance
import pandas as pd
dataFrame = pd.read_excel("/home/shivam/hackathon/AbInBev/Focus Area - Image Processing/ABI_Price_List.xlsx", sheetname = "Sheet2")


def get_knowledge_price():
    price = {}
    count = 0
    for index, row in dataFrame.iterrows():
        count += 1
        # print count, (str(row['Date'].date()), row['Brand'], row['SKU'])
        price[(str(row['Date'].date()), row['Brand'], row['SKU'])] = row['Recommended Price']

    # print price[('2015-03-29', u'LEFFE BLONDE', u'6 Pk 12 Oz Glass')]
    return price

def get_brand_knowledge():
    brand_list = []
    for object in dataFrame['Brand']:
        brand_list.append(object)

    brand_list = set(brand_list)
    lst = []
    for object in brand_list:
        lst.append(object)

    return lst


def get_sku_knowledge():
    sku_list = []
    for object in dataFrame['SKU']:
        sku_list.append(object)

    sku_list = set(sku_list)
    lst = []
    for object in sku_list:
        lst.append(object)

    return lst


def match_brand(brand):
    brand_knowledge = get_brand_knowledge()
    ratio = []
    for object in brand_knowledge:
        ratio.append(editdistance.eval(object, brand))

    index = ratio.index(min(ratio))
    return brand_knowledge[index]


def match_sku(sku):
    # print sku
    sku_knowledge = get_sku_knowledge()
    ratio = []
    for object in sku_knowledge:
        ratio.append(editdistance.eval(object, sku))

    # for i in range(0, len(ratio)):
    #     print ratio[i], sku_knowledge[i]
    index = ratio.index(min(ratio))
    #print sku_knowledge[index]
    return sku_knowledge[index]


def read_files(fname, count):
    print fname
    with open(fname,'r') as f:
        content = f.readlines()
        content = [x.strip() for x in content]
        content = filter(None, content)
        content[0] = match_brand(content[0])
        try:
            content[1] = match_sku(content[1])
        except:
            count += 1
            return "LMAO", count
        # print content
        try:
            content[2] = content[2].split('$')[1]
            x = list(content[2])
            for i in range(len(x)):
                if x[i] == 'a'or x[i]=='A':
                    x[i]='4'
                if x[i] == 's' or x[i] == 'g' or x[i]=='S':
                    x[i] ='9'
            # content[2] = [i.strip() for i in content[2]]
            content[2] = ''.join(x)
            content[2] = content[2].replace(" ", "")
            a, b = content[2][:len(content[2])-2], content[2][-2:]
            # print a,b
            x = a + "." + b
            content[2] = ''.join(x)
            return content, count
        except:
            # print fname
            count += 1
            return "LMAO", count

def create_list(mypath):
    key_error_count = 0
    count = 0
    csvfile = open("some.csv","wb")
    csvwriter = csv.writer(csvfile,delimiter=',')
    csvwriter.writerow(['Fname','Date','Image Beer','Actual Cost', 'Tag Beer', 'SKU', 'Price Beer'])
    date_shelf ={}
    price = get_knowledge_price()
    for file in listdir(mypath):
        if file.endswith(".txt"):
            file_name = file[12:-4]
            if file_name[:12] in date_shelf:
                date_shelf[file_name[:12]].append(file_name[13:])
            else:
                date_shelf[file_name[:12]] = [file_name[13:]]

    centroid_products = centroid.find_centroid()
    for i in date_shelf:
        date_shelf[i].sort()
        for j in date_shelf[i]:
            minim = 10000000
            k_val = 0
            for k in centroid_products:
                #print "HI", i[-1], k[0]
                if i[-1] == k[0]:
                    if abs(int(j[2:])-centroid_products[k][0])< minim:
                        minim = abs(int(j[2:])-centroid_products[k][0])
                        k_val = k
            # print centroid_products[k_val][1]
            # print mypath+"Supermarket_"+i+"_"+j+".txt"
            content, count = read_files(mypath+"Supermarket_"+i+"_"+j+".txt", count)
            if content!="LMAO":
                # t = i[:-2].split('-')
                # m=t[2]+"/"+t[1]+"/"+t[0]
                m = i[:-2]
                actual_price = (m, content[0], content[1])
                try:
                    # print price[actual_price]
                    csvwriter.writerow([mypath+"Supermarket_"+i+"_"+j+".txt",m,centroid_products[k_val][1], price[actual_price]]+content, )
                except:
                    print actual_price
                    key_error_count += 1
                    # csvwriter.writerow([mypath+"Supermarket_"+i+"_"+j+".txt",m,centroid_products[k_val][1], "Not Found" ]+content, )
    print count
    print key_error_count
#read_files("output_text/")
create_list("final_t10_2/")
# match_sku(u"an! 5 PK 12 Oz G‘ass")
# get_knowledge_price()
# print match_sku('“211” 5 Pk 12 oz G‘ass')





