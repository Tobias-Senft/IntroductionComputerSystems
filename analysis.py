import pandas as pd
baicArray = pd.DataFrame([(1,292,6.039), 
             (2 ,287 ,4.680),
             (1 ,291 ,4.595),
             (1 ,292 ,4.670 ),
             (1 ,290 ,7.818 ),
             (1 ,294 ,4.755),
             (2 ,267 ,5.257 ),
             (2 ,254 ,7.818)
             ])
dynamicArray = pd.DataFrame( [(3 , 266 , 7.338), 
               (2 ,269 ,6.022 ),
               (3 ,265 ,7.018), 
               (3 ,265 ,6.503),
               (3 ,263 ,7.893),
               (3 ,272 ,7.904),
               (3 ,251 ,6.249),
               (2 ,253 ,11.537 )
               ])
print("Basic Array")
print(baicArray.describe())
print("Dynamic Array")
print(dynamicArray.describe())


  
  