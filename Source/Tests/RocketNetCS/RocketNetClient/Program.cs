using System.Runtime.InteropServices;
using RocketNet;
using System.Threading;

RocketNetClient client = new RocketNetClient();
client.Run();

class RocketNetClient
{
    static string _input = "";
    private static bool _inputReceived = false;

    public void RunAsyncInputListener()
    {
        Thread userThread = new Thread(new ThreadStart(UserInput));
        userThread.Start();
    }

    static void UserInput()
    {
        while (true)
        {
            if (!_inputReceived)
            {
                _input = Console.ReadLine();
                if (!string.IsNullOrEmpty(_input))
                {
                    _inputReceived = true;
                }
            }
        }
    }

    public void Run()
    {
        RunAsyncInputListener();
        var rocketNet = new RocketNet.RocketNetInstance();
        rocketNet.StartClient("127.0.0.1", 60000, "");

        while (true)
        {
            if (_inputReceived)
            {
                string latestInput = _input;
                _inputReceived = false;

                if (latestInput == "quit")
                {
                    
                }
                else
                {
                    IntPtr inputDataPtr = Marshal.StringToHGlobalAnsi(latestInput);
                    rocketNet.SendDataToHost(inputDataPtr, true, 135);
                }
            }
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
                            foreach (var GUID in connectionGUIDs)
                            {
                                Console.Write(GUID + " | ");
                            }
                            Console.WriteLine();
                            Console.WriteLine(packetDataAsString);
                        }
                    }
                }
            }

            rocketNet.ClearPendingPackets();
        }
    }
};