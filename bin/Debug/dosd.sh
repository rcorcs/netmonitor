source setup.sh

detect()
{
case $INTERFACE in
"DEFAULT")
   sudo $PROGRAM -d $DOSC_FILE;;
*)
   sudo $PROGRAM -d $DOSC_FILE -i $INTERFACE;;
esac
}

train()
{
case $INTERFACE in
"DEFAULT")
   sudo $PROGRAM -t $DOSC_FILE $TCPSYN_NORMAL_FILE $TCPSYN_ATTACK_FILE $UDP_NORMAL_FILE $UDP_ATTACK_FILE;;
*)
   sudo $PROGRAM -t $DOSC_FILE $TCPSYN_NORMAL_FILE $TCPSYN_ATTACK_FILE $UDP_NORMAL_FILE $UDP_ATTACK_FILE -i $INTERFACE;;
esac
}

store()
{
case $INTERFACE in
"DEFAULT")
   sudo $PROGRAM -s $TCP_DATA_FILE $UDP_DATA_FILE;;
*)
   sudo $PROGRAM -s $TCP_DATA_FILE $UDP_DATA_FILE -i $INTERFACE;;
esac
}

load()
{
case $INTERFACE in
"DEFAULT")
   sudo $PROGRAM -l $TCP_DATA_FILE $UDP_DATA_FILE;;
*)
   sudo $PROGRAM -l $TCP_DATA_FILE $UDP_DATA_FILE -i $INTERFACE;;
esac
}

sniffer()
{
case $INTERFACE in
"DEFAULT")
  sudo $PROGRAM -S;;
*)
  sudo $PROGRAM -S -i $INTERFACE;;
esac
}

debug()
{
case $INTERFACE in
"DEFAULT")
   sudo $PROGRAM -P;;
*)
   sudo $PROGRAM -P -i $INTERFACE;;
esac
}

host()
{
   sudo $PROGRAM -h
}

stop()
{
   sudo killall $PROGRAM
}

case $# in
0)
   detect ;;
*)

case $1 in
"detect")
   detect ;;
"train")
   train ;;
"store")
   store ;;
"load")
   load ;;
"debug")
   debug ;;
"sniffer")
   sniffer ;;
"stop")
   stop ;;
"host")
   host ;;
*)
   echo "Arguments not found: $*"
esac

esac
