path=$(pwd)

echo "kill apps"

kill -9 `ps aux | grep -v grep | grep brght | grep $path | awk ' {print $2}' `

