path=./

echo "echo apps"

ps aux | grep -v grep | grep brght |grep -v "sshd\: brght"
#kill -9 `ps aux | grep -v grep | grep brght | grep local_feat_calc | awk '{print $2}'`

#sleep 3
#echo "ps apps after killing"
#ps aux | grep brght |  grep -v grep| grep ${path}controller

