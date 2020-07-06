using System;
using System.Net;
using System.Threading;

namespace SimpleDotNetClient
{
    class Program
    {
        static void Main(string[] args)
        {
            int counter = 0;
            int logLaps = 10;

            int clickPerSecond = 300;

            int laps = 1000 / clickPerSecond;

            while (true)
            {
                WebRequest request = WebRequest.Create("http://localhost:1414/AutoClicker?Click=1");
                HttpWebResponse response = (HttpWebResponse)request.GetResponse();

                counter++;
                if (counter >= logLaps)
                {
                    Console.WriteLine($"Request response.");
                    counter = 0;
                }

                // limit the speed.
                if (laps > 0)
                {
                    Thread.Sleep(laps);
                }
            }
        }
    }
}
