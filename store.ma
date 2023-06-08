[top]
components : generator@Generator distributor@Distributor cash0@Cash cash1@Cash cash2@Cash cash3@Cash 
components : traveler@Traveler CarSystem
out : customerFinished bagRemoved

Link : holdOff@distributor holdOff@generator
Link : okGo@distributor okGo@generator
Link : carOrNot@generator carOrNot@distributor 
Link : searchTimeDistribution@generator searchTimeDistribution@distributor 
Link : traveltimeDistribution@generator travelTimeDistribution@distributor  
Link : numberOfItemsDistribution@generator numberOfItemsDistribution@distributor  

Link : outputCarOrNot@distributor carOrNot@cash0 
Link : outputCarOrNot@distributor carOrNot@cash1 
Link : outputCarOrNot@distributor carOrNot@cash2 
Link : outputCarOrNot@distributor carOrNot@cash3 

Link : outputWhichCash@distributor whichCash@cash0 
Link : outputWhichCash@distributor whichCash@cash1 
Link : outputWhichCash@distributor whichCash@cash2 
Link : outputWhichCash@distributor whichCash@cash3 

Link : outputSearchTime@distributor searchTimeDistribution@cash0 
Link : outputSearchTime@distributor searchTimeDistribution@cash1 
Link : outputSearchTime@distributor searchTimeDistribution@cash2 
Link : outputSearchTime@distributor searchTimeDistribution@cash3 

Link : outputNumberOfItems@distributor numberOfItemsDistribution@cash0 
Link : outputNumberOfItems@distributor numberOfItemsDistribution@cash1 
Link : outputNumberOfItems@distributor numberOfItemsDistribution@cash2 
Link : outputNumberOfItems@distributor numberOfItemsDistribution@cash3 

Link : outputTravelTime@distributor travelTimeDistribution@cash0 
Link : outputTravelTime@distributor travelTimeDistribution@cash1 
Link : outputTravelTime@distributor travelTimeDistribution@cash2 
Link : outputTravelTime@distributor travelTimeDistribution@cash3 

Link : oneLessCustomer@cash0 oneLessCustomer@distributor
Link : oneLessCustomer@cash1 oneLessCustomer@distributor
Link : oneLessCustomer@cash2 oneLessCustomer@distributor
Link : oneLessCustomer@cash3 oneLessCustomer@distributor

Link : bagSearchTime@cash0 bagSearchTime@carsystem bagSearchTime@cash1 bagSearchTime@carsystem  
Link : bagSearchTime@cash2 bagSearchTime@carsystem bagSearchTime@cash3 bagSearchTime@carsystem 

Link : customerFinished@cash0 customerFinished
Link : customerFinished@cash1 customerFinished
Link : customerFinished@cash2 customerFinished
Link : customerFinished@cash3 customerFinished

Link : outputTravelTime@cash0 travelTimeDistribution@traveler
Link : outputTravelTime@cash1 travelTimeDistribution@traveler
Link : outputTravelTime@cash2 travelTimeDistribution@traveler
Link : outputTravelTime@cash3 travelTimeDistribution@traveler

Link : customerOutput@cash0 customerId@traveler
Link : customerOutput@cash1 customerId@traveler
Link : customerOutput@cash2 customerId@traveler
Link : customerOutput@cash3 customerId@traveler

Link : customerOutput@traveler customerId@CarSystem
Link : customerFinished@CarSystem customerFinished
Link : bagRemoved@CarSystem bagRemoved
Link : bagId@cash0 bagId@CarSystem
Link : bagId@cash1 bagId@CarSystem
Link : bagId@cash2 bagId@CarSystem
Link : bagId@cash3 bagId@CarSystem

[CarSystem]
components : conveyor@Conveyor searcher@Searcher
out : customerFinished bagRemoved
in : customerId bagSearchTime bagId
link : bagId bagId@conveyor
link : customerId customerId@searcher
link : bagSearchTime bagSearchTime@searcher
link : customerFinished@searcher customerFinished 
link : removeBag@searcher removeBag@conveyor
link : bagRemoved@conveyor bagRemoved

[generator]
arrivalDistribution : exponential
searchTimeDistribution : exponential
travelTimeDistribution : normal
numberOfItemsDistribution : normal

arrivalAverage : 10
searchTimeAverage : 5
travelTimeAverage : 10
travelTimeStandardError : 1
numberOfItemsAverage : 100
numberOfItemsStandardError : 15
