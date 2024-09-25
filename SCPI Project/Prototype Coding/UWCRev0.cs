using System;

using System.Collections.Generic;

using System.Text;

using System.Net.Sockets;

using System.IO;

namespace Network.Connect

{

    class Program

    {

        static TelnetConnection tc;

        static int Main(string[] args)

        {

            // defaultHostName is host name to use if one is not specified on the command line.

            string defaultHostName = "192.168.1.1";

            string hostName = defaultHostName;

            if( args.Length == 1 )

            {

                // If command line contains a '?' character, interpret this as help.

                if( args[0].Contains("?"))

                {

                    Console.WriteLine("Usage: N9912A_CS_Example.exe <hostName>\n\n"+

                        "Where optional hostName is an ip address or host name.\n" +

                        "If no hostName is supplied, the default ("+defaultHostName+") is used.\n\n"+

                        "e.g. N9912A_CS_Example.exe 10.10.1.1\n\nor\n\n" +

                        "N9912A_CS_Example.exe A-N9912A-22762");

                    return 0; // exit.

                }

                // Record hostname passed in on command line.

                hostName = args[0];

            }

            try

            {

                tc = new TelnetConnection();

                tc.ReadTimeout = 10000; // 10 sec

                // open socket on hostName, which can be an IP address, or use host name (e.g. "A-N9912A-22762") used in lieu of IP address

                tc.Open(hostName);

                if( tc.IsOpen )

                {

                    //Start your program here
					string []UnitCheck={"Wheel Locked ?","Levelling Done ?","Panel Installed ?","Unit screw Torqued ?","Test Foam Installed ?"};
               		Console.WriteLine("Welcome To UWC FieldFox Console");
					Console.WriteLine("Would You Like to Ensure Unit Mechanically Ready ?(Y/N)");
					if (Console.ReadKey().Key != ConsoleKey.N)
					{
					goto Prelim;
					}
					if(Console.ReadKey().Key !=ConsoleKey.Y){
					UnitCheck:
					int i=0;
					Console.WriteLine(UnitCheck[0+i]+"(Y/N)");
					if(Console.ReadKey().Key !=ConsoleKey.Y){
					i=i+1;
					goto UnitCheck;
					}
					if(i>=4){
					goto Prelim;
					}
					else{
					i=i+0;
					goto UnitCheck;
					}
					
					}					
					Prelim:
					String Date;
					String Unit;
					Console.WriteLine("Enter Today's Date: DDMMYYYY");
					Date=Console.ReadLine();
					Console.WriteLine("Enter Unit Number");
					Unit=Console.ReadLine();
					//Auto Power ON
					Write("SYST:POWER:AUTO;*OPC?");
					Console.WriteLine("POWER AUTO");
					//Measurement Setting                    
					Write("INST:SEL:NA;*OPC?");
					Console.WriteLine("Network Analysis Mode");
					Write("CALC:PAR2:DEF S21");
					Console.WriteLine("S21 Mode");
					Write("CALC:PAR:COUN 1");
					Console.WriteLine("Trace Set");
					Write("BWID 30");
					Console.WriteLine("Bandwidth 30Hz");
					Write("SWE:POIN 871");
					Console.WriteLine("871 Resolution Set");
					Write("SWE:POIN 101");
					Console.WriteLine("101 Resolution Set");
					Write("FREQ:STAR 600E6");
					Console.WriteLine("Start Frequency 600MHz");
					Write("FREQ:STOP 18E9");
					Console.WriteLine("Stop Frequency 18GHz");
					Write("DISP:WIND:TRAC2:Y:PDIV 20");
					Console.WriteLine("dB/Division 20dB");
					Write("DISP:WIND:TRAC1:Y:RPOS -60");
					Console.WriteLine("Scale reference -60dB");
					Write("DISP:MARK:LARG:STAT A");
					Write("DISP:MARK:LARG:A:DEF:TRAC1:MEAS S21");
					Write("DISP:MARK:LARG:A:DEF:LINE1:TRACking 1");
					Console.WriteLine("Big readout Set");
					Write("SOUR: POW 0");
					Console.WriteLine("Output Power Set to 0dB");
					//Limit Set
					Write("CALC:LIM:LLD 1,1,0,0,4,600E6,-70,7.125E9,-80,18E9,-80");
					Console.WriteLine("Limit Set ");
					//Calibration Loop
					Calibration:
						Console.WriteLine("Are both the TX and RX Power Amplifier Turned ON ?(Y/N)");
					if (Console.ReadKey().Key != ConsoleKey.N)
					{
						Console.WriteLine("Please Turn On TX and RX Power Amplifier before calibration");
						goto Calibration;
					}

					if (Console.ReadKey().Key != ConsoleKey.Y)
					{
						Console.WriteLine("Initiating Calibration...");
						Write("SWE:POIN// 101");
						Write("SENS:CORR:COLL:METH:THRU 1,2");
						Write("-p Attach thru now");
						Write("SENS:CORR:COLL:THRU 1,2;*OPC?");
						Write("SENS:CORR:COLL:SAVE 0");
						Console.WriteLine("101 Calibration done");
						Write("SWE:POIN 871");
						Write("SENS:CORR:COLL:METH:THRU 1,2");
						Write("-p Attach thru now");
						Write("SENS:CORR:COLL:THRU 1,2;*OPC?");
						Write("SENS:CORR:COLL:SAVE 0");
						Console.WriteLine("101 Calibration done");
						goto Ready;
					}

					Ready:
					String USBDISK;
					USBDISK="[USBDISK]";
					Write("MMEM:CDIR"+USBDISK);
					Console.WriteLine("Ready for Isolation Test");
					goto Measurement;
					Measurement:
					Console.WriteLine("Please Move Transmitter and Receiver to Correct Test Point");
					Console.WriteLine("Are both the Transmitter and Receiver at the Correct Test Point ?(Y/N)");
					if(Console.ReadKey().Key != ConsoleKey.N){
					goto Measurement;
					}
					if(Console.ReadKey().Key != ConsoleKey.Y){
					Console.WriteLine("Enter Test Code");
					String TestCode;
					TestCode=Console.ReadLine();
					Console.WriteLine("Enter Resolution Mode (101/871)");
					String Resolution;
					Resolution=Console.ReadLine();
					if(Resolution=="101"){
						Write("SWE:POIN 101");
						goto DataReady;
					}
					if(Resolution=="871"){
						Write("SWE:POIN 871");
						goto DataReady;
					}	
					DataReady:
					Console.WriteLine("Wait for 3 full sweep...");
				   	Console.WriteLine("Data Ready or Hold?(Y/N/H)");
					if(Console.ReadKey().Key != ConsoleKey.N){
					goto DataReady;
					}
					if(Console.ReadKey().Key!=ConsoleKey.H){
					Write("INIT:CONT 0;*OPC?");
					Hold:
					Console.Write("Hold Ongoing..,Stop hold ?(Y/N)");
					if(Console.ReadKey().Key!=ConsoleKey.Y)
					{
					Write("INIT:CONT 1;*OPC?");
					Console.WriteLine("Hold Off");
					goto DataReady;
					}
					if(Console.ReadKey().Key!=ConsoleKey.N)
					{
					goto Hold;
					}
					}
					if(Console.ReadKey().Key != ConsoleKey.Y){
					Console.WriteLine("Saving File...");
					String FileName;
					String FullPNG;
					String FullCSV;
					FileName=Unit+"."+TestCode+"."+Resolution+"."+Date;
					FullCSV="MMEM:STOR:FDAT"+FileName+".csv";
					Write(FullCSV);
					FullPNG="MMEM:STOR:FDAT"+FileName+".png";
					Write(FullPNG);
					Console.WriteLine("File Have been Saved");
					Console.WriteLine("Continue Testing ?(Y/N)");
					if(Console.ReadKey().Key != ConsoleKey.N){
					goto shutdown;
					}
					if(Console.ReadKey().Key != ConsoleKey.Y){
					goto Measurement;
					}
					
					}	
				
				    
					}
					shutdown:
					Console.Write("Shutting down FieldFox...");
					Write("SYST:PWR:SHUT");

                    //End your program here

                    tc.Dispose();

                    Console.WriteLine("Press any key to exit.");

                    Console.ReadKey(); // continue after reading a key from the keyboard.

                }

                else

                {

                    Console.WriteLine("Error opening " + hostName);

                    return -1;

                }

                //FieldFox Programming Guide 5

            }

            catch(Exception e)

            {

                Console.WriteLine(e.ToString());

                return -1;

            }

            // exit normally.

            return 0;

        }

        /// <summary>

        /// Write a SCPI command to the telnet connection.

        /// If the command has a '?', then read back the response and print

        /// it to the Console.

        /// </summary>

        /// <remarks>

        /// Note the '?' detection is naive, as a ? could occur in the middle

        /// of a SCPI string argument, and not actually signify a SCPI query.

        /// </remarks>

        /// <param name="s"></param>

        static void Write(string s)

        {

            Console.WriteLine(s);

            tc.WriteLine(s);

            if (s.IndexOf('?') >= 0)

                Read();

        }

        /// <summary>

        /// Read the telnet connection for a response, and print the response to the

        /// Console.

        /// </summary>

        static void Read()

        {

            Console.WriteLine(tc.Read());

        }

    }

#region TelnetConnection - no need to edit

    /// <summary>

    /// Telnet Connection on port 5025 to an instrument

    /// </summary>

    public class TelnetConnection : IDisposable

    {

        TcpClient m_Client;

        NetworkStream m_Stream;

        bool m_IsOpen = false;

        string m_Hostname;

        int m_ReadTimeout = 1000; // ms

        public delegate void ConnectionDelegate();

        public event ConnectionDelegate Opened;

        public event ConnectionDelegate Closed;

        public bool IsOpen { get { return m_IsOpen; } }

        public TelnetConnection() { }

        public TelnetConnection(bool open) : this("localhost", true) { }

        public TelnetConnection(string host, bool open)

        {

            if (open)

                Open(host);

        }

        void CheckOpen()

        {

            if (!IsOpen)

                throw new Exception("Connection not open.");

        }

        public string Hostname

        {

            get { return m_Hostname; }

        }

        public int ReadTimeout

        {

            set { m_ReadTimeout = value; if (IsOpen) m_Stream.ReadTimeout = value; }

            get { return m_ReadTimeout; }

        }

        public void Write(string str)

        {

            //FieldFox Programming Guide 6

            CheckOpen();

            byte[] bytes = System.Text.ASCIIEncoding.ASCII.GetBytes(str);

            m_Stream.Write(bytes, 0, bytes.Length);

            m_Stream.Flush();

        }

        public void WriteLine(string str)

        {

            CheckOpen();

            byte[] bytes = System.Text.ASCIIEncoding.ASCII.GetBytes(str);

            m_Stream.Write(bytes, 0, bytes.Length);

            WriteTerminator();

        }

        void WriteTerminator()

        {

            byte[] bytes = System.Text.ASCIIEncoding.ASCII.GetBytes("\r\n\0");

            m_Stream.Write(bytes, 0, bytes.Length);

            m_Stream.Flush();

        }

        public string Read()

        {

            CheckOpen();

            return System.Text.ASCIIEncoding.ASCII.GetString(ReadBytes());

        }

        /// <summary>

        /// Reads bytes from the socket and returns them as a byte[].

        /// </summary>

        /// <returns></returns>

        public byte[] ReadBytes()

        {

            int i = m_Stream.ReadByte();

            byte b = (byte)i;

            int bytesToRead = 0;

            var bytes = new List<byte>();

            if ((char)b == '#')

            {

                bytesToRead = ReadLengthHeader();

                if (bytesToRead > 0)

                {

                    i = m_Stream.ReadByte();

                    if ((char)i != '\n') // discard carriage return after length header.

                        bytes.Add((byte)i);

                }

            }

            if (bytesToRead == 0)

            {

                while (i != -1 && b != (byte)'\n')

                {

                    bytes.Add(b);

                    i = m_Stream.ReadByte();

                    b = (byte)i;

                }

            }

            else

            {

                int bytesRead = 0;

                while (bytesRead < bytesToRead && i != -1)

                {

                    i = m_Stream.ReadByte();

                    if (i != -1)

                    {

                        bytesRead++;

                        // record all bytes except \n if it is the last char.

                        if (bytesRead < bytesToRead || (char)i != '\n')

                            bytes.Add((byte)i);

                    }

                }

            }

            return bytes.ToArray();

        }

        int ReadLengthHeader()

        {

            int numDigits = Convert.ToInt32(new string(new char[] { (char)m_Stream.ReadByte() }));

            string bytes = "";

            for (int i = 0; i < numDigits; ++i)

                bytes = bytes + (char)m_Stream.ReadByte();

            return Convert.ToInt32(bytes);

        }

        public void Open(string hostname)

        {

            if (IsOpen)

                Close();

            m_Hostname = hostname;

            m_Client = new TcpClient(hostname, 5025);

            m_Stream = m_Client.GetStream();

            m_Stream.ReadTimeout = ReadTimeout;

            m_IsOpen = true;

            if (Opened != null)

                Opened();

        }

        public void Close()

        {

            if (!m_IsOpen)

                //FieldFox Programming Guide 7

                return;

            m_Stream.Close();

            m_Client.Close();

            m_IsOpen = false;

            if (Closed != null)

                Closed();

        }

        public void Dispose()

        {

            Close();

        }

    }

#endregion

}