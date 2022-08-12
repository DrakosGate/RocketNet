using RocketNet;

var rocketNet = new RocketNet.RocketNetInstance();
rocketNet.StartHost(60000, "");

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
