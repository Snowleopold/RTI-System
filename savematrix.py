import numpy as np
import math as mt
import os
os.remove('tikweights.npy')
print("Old matrix deleted")
nodenum=16
axis1=101
axis2=101
size=(nodenum*(nodenum-1))
linknum=80
pts=np.array([[0,0,25,100],[0,0,50,100],[0,0,75,100],[0,0,100,100],[0,0,100,75],[0,0,100,50],[0,0,100,25],[0,25,25,100],[0,25,50,100],[0,25,75,100],[0,25,100,100],[0,25,100,75],[0,25,100,50],[0,25,100,25],[0,25,100,0],[0,25,75,0],[0,25,50,0],[0,25,25,0],[0,50,25,100],[0,50,50,100],[0,50,75,100],[0,50,100,100],[0,50,100,75],[0,50,100,50],[0,50,100,25],[0,50,100,0],[0,50,75,0],[0,50,50,0],[0,50,25,0],[0,75,25,100],[0,75,50,100],[0,75,75,100],[0,75,100,100],[0,75,100,75],[0,75,100,50],[0,75,100,25],[0,75,100,0],[0,75,75,0],[0,75,50,0],[0,75,25,0],[0,100,100,75],[0,100,100,50],[0,100,100,25],[0,100,100,0],[0,100,75,0],[0,100,50,0],[0,100,25,0],[25,100,100,75],[25,100,100,50],[25,100,100,25],[25,100,100,0],[25,100,75,0],[25,100,50,0],[25,100,25,0],[50,100,100,75],[50,100,100,50],[50,100,100,25],[50,100,100,0],[50,100,75,0],[50,100,50,0],[50,100,25,0],[75,100,100,75],[75,100,100,50],[75,100,100,25],[75,100,100,0],[75,100,75,0],[75,100,50,0],[75,100,25,0],[100,100,75,0],[100,100,50,0],[100,100,25,0],[100,75,75,0],[100,75,50,0],[100,75,25,0],[100,50,75,0],[100,50,50,0],[100,50,25,0],[100,25,75,0],[100,25,50,0],[100,25,25,0]])
pict1=np.zeros((linknum,axis1,axis2)).astype(float)
lamb=0.1
alpha=0.001

def euclid(x1,x2,y1,y2):
    return mt.sqrt((x1-x2)**2+(y1-y2)**2)

for i in range(linknum):
    dist=euclid(pts[i,0],pts[i,2],pts[i,1],pts[i,3])
    arr=np.zeros((axis1,axis2)).astype(float)
    for x in range(axis1):
        for y in range(axis2):         
            if(euclid(x,pts[i,0],y,pts[i,1])+euclid(x,pts[i,2],y,pts[i,3])<(dist+lamb)):
                arr[x,y]=1/mt.sqrt(dist)
    pict1[i]=arr

print("Calculating weights...")
pictre=pict1.reshape((linknum,axis1*axis2))
weights=np.matmul(np.transpose(pictre),pictre)+alpha*np.identity(axis1*axis2)
weights=np.matmul(np.linalg.inv(weights),np.transpose(pictre))
np.save('tikweights',weights)
print(np.max(weights))
print(np.min(weights))
print(np.median(weights))
print(weights.shape)
print("Weights calculate complete")
