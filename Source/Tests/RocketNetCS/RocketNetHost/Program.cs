using RocketNet;

var rocketNet = new RocketNet.RocketNetInstance();
rocketNet.StartHost(60000, "");

while (true)
{
    rocketNet.FetchPendingPackets();
    rocketNet.ClearPendingPackets();
}
