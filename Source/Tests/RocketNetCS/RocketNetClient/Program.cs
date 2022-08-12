using RocketNet;

var rocketNet = new RocketNet.RocketNetInstance();
rocketNet.StartClient("127.0.0.1", 60000, "");

while (true)
{
    var connectionGUIDs = rocketNet.GetConnectionGUIDs();
    if (rocketNet.CollectPendingPackets())
    {
        while (rocketNet.HandleNextPendingPacket())
        {

        }
    }

    rocketNet.ClearPendingPackets();
}