import json
import boto3
import urllib

rekognition = boto3.client('rekognition')
iot = boto3.client('iot-data')
s3 = boto3.client('s3')

data_of_interest = {'gender':"",
                    'emotion':"",
                    'smile':False,
                    'beard':False,
                    'mostache':False,
                    'glasses':False,
                    'age_high':"",
                    'age_low':""
}

# add uploaded face to collection
def addFaceToCollection(bucket, event_key, collection_name):
    print("Adding face to collection")
    try:
        externalID = event_key.strip('inv_corta3/')
        externalID = externalID.strip('.jpg')
        
        add_response = rekognition.index_faces(
            CollectionId=collection_name,
            DetectionAttributes=['ALL'],
            ExternalImageId=externalID,
            Image={
                'S3Object': {
                    'Bucket' : bucket,
                    'Name' : event_key,
                },
            },
        )
        
        # print(add_response)
        
        data_of_interest['gender'] = add_response['FaceRecords'][0]['FaceDetail']['Gender']['Value']
        data_of_interest['emotion'] = add_response['FaceRecords'][0]['FaceDetail']['Emotions'][0]['Type']
        data_of_interest['beard'] = add_response['FaceRecords'][0]['FaceDetail']['Beard']['Value']
        data_of_interest['mostache'] = add_response['FaceRecords'][0]['FaceDetail']['Mustache']['Value']
        data_of_interest['glasses'] = add_response['FaceRecords'][0]['FaceDetail']['Eyeglasses']['Value']
        data_of_interest['smile'] = add_response['FaceRecords'][0]['FaceDetail']['Smile']['Value']
        data_of_interest['age_high'] = add_response['FaceRecords'][0]['FaceDetail']['AgeRange']['High']
        data_of_interest['age_low'] = add_response['FaceRecords'][0]['FaceDetail']['AgeRange']['Low']
        
        print("--> Image: " + externalID)
        print(data_of_interest)

    except Exception as e:
        print(e)
        print("ERROR: " + event_key)
        raise e 

# entry point
def lambda_handler(event, context):
    print("MP6122 - Inv. Corta 3")
    bucket = event['Records'][0]['s3']['bucket']['name']
    event_img_key = urllib.unquote_plus(event['Records'][0]['s3']['object']['key'].encode('utf8'))
    
    #rekognition.create_collection(CollectionId='mp6122_collection')
    addFaceToCollection(bucket, event_img_key, 'mp6122_collection')