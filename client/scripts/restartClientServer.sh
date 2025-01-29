name=$(whoami)
S1=$(ps -u "$name" | grep runClient)
S2=""

date_time=$(date +"%Y-%m-%d_%H-%M-%S")
echo $date_time
mkdir -p logs

if [ "$S1" == "$S2" ];
then
  echo "Client Server STARTED..  "

	if [ "$(find . -maxdepth 1 -name "*.json" -print -quit)" ]; 
  then
    mkdir -p archiveFileOutputs
    
    file=$(find . -maxdepth 1 -name "*.json" -print -quit)
    filename=$(basename "$file")

    mv "$file" "archiveFileOutputs/${filename%.*}_$date_time.json"
	fi
 
  ./runClient | tee -a "logs/clientLog_${date_time}.txt"
else
  echo "Client Server is already running do you want to restart it y or n  "
  read ans
  if [ "$ans" == "y" ]
  then
    pkill runClient
    echo "Client Server RESTARTED..  "
      
    if [ "$(find . -maxdepth 1 -name "*.json" -print -quit)" ]; 
    then
    	mkdir -p archiveFileOutputs
    	
    file=$(find . -maxdepth 1 -name "*.json" -print -quit)
    filename=$(basename "$json_file")

    mv "$file" "archiveFileOutputs/${filename%.*}_$date_time.json"
                 
		fi


    ./runClient | tee -a "logs/clientLog_${date_time}.txt"
     else
    echo "Old instance is running..."
  fi
fi

