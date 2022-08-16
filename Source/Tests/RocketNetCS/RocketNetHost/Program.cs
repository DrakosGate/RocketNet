using System.Runtime.InteropServices;
using RocketNet;

var rocketNet = new RocketNet.RocketNetInstance();
rocketNet.StartHost(60000, "");

while (true)
{
    var connectionGUIDs = rocketNet.GetConnectionGUIDs();
    if (rocketNet.CollectPendingPackets())
    {
        RocketNetPacket unhandledPacket = new RocketNetPacket();
        while (rocketNet.HandleNextPendingPacket(ref unhandledPacket))
        {
            if (unhandledPacket.data != IntPtr.Zero)
            {
                string packetDataAsString = Marshal.PtrToStringAnsi(unhandledPacket.data);
                if (!string.IsNullOrEmpty(packetDataAsString))
                {
                    if (unhandledPacket.packetID == 0)
                    {
                        Console.WriteLine("Packet received from " + unhandledPacket.senderGUID + ": " + packetDataAsString);
                        foreach (var guid in connectionGUIDs)
                        {
                            if (guid != unhandledPacket.senderGUID)
                            {
                                rocketNet.SendDataToConnection(guid, unhandledPacket.data, true, unhandledPacket.packetID);
                            }
                        }
                    }
                }
            }
        }
    }

    rocketNet.ClearPendingPackets();
}