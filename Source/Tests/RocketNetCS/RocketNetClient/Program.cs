using RocketNet;

var rocketNet = new RocketNet.RocketNetInstance();
rocketNet.StartClient("127.0.0.1", 60000, "");
while (true)
{
    rocketNet.ProcessPendingClientPackets();
}