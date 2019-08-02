import sys
import logging
import time
import getopt
import os
import tinys3
import json

# Read in command-line parameters
privateKeyPath = ""
imgName = ""
datapath = "/home/parisfe/Maestria/MP6122_Reconocimiento_de_Patrones/InvestigacionCorta3/imgs/"

try:
    opts, args = getopt.getopt(sys.argv[1:], "hwe:i:", ["help","img="])
    if len(opts) == 0:
            raise getopt.GetoptError("No input parameters!")
    for opt, arg in opts:
            if opt in ("-h", "--help"):
                    print(helpInfo)
                    exit(0)
            if opt in ("-i", "--img"):
                    imgName = arg
			
except getopt.GetoptError:
    print(usageInfo)
    exit(1)

# AWS S3 properties
access_key_id = ''
secret_access_key = ''
bucket_name = 'mp6122'

# Upload photo to S3
def uploadToS3():
    filepath = "inv_corta3/" + imgName
    conn = tinys3.Connection("AKIAJTS4KLZBNSU7MTUA", "NNryYU+xClECTrio1HnV6vN07ny7WKNUN0mbG8sx")

    # open src files    
    img = open(datapath+imgName, 'rb')

    # upload data to s3
    conn.upload(filepath, img, bucket_name)

    # close src files
    img.close()

uploadToS3()
sys.exit()
