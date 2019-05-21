README.txt
==========

version 0.1 (mds) - Initial creation

==============================================================================

This zip folder contains the following files:

"USB Interface Adapter GUI-v1.10.zip" - USB Interface Adapter reference GUI
"PMBus Reference GUI.zip" - PMBus reference GUI
"USB Interface Adapter Driver.zip" - .NET DLL for use with USB Interface Adapter

The two GUIs included are Windows .NET GUIs. They require the Microsoft Framework version 2.0 to be installed on the machine that will run them. There seems to be a requirement to run them from a locally attached drive (C:), rather than from a virtual filesystem on your PC. To run the GUIs, unzip the file to your PC's hard drive and run the executable file (*** not the one with vshost in the name, but either "USB SAA GUI.exe" or "PMBusDebug.exe".

In order to tell if your machine has the .NET Framework version 2.0, you can go to the control panel and select "Add/Remove Programs". Under there, you should see which version of the framework your machine has. Most recent machines have 2.0, but if your PC is older, you will probably have version 1.0. To upgrade, go to the following URL at Microsoft: "http://www.microsoft.com/downloads/details.aspx?FamilyID=0856eacb-4362-4b0d-8edd-aab15c5e04f5&displaylang=en".

USB Interface Adapter GUI
=========================

This GUI is intended to support low-level I2C, SMBus and PMBus transactions between a master (the USB interface adapter) and a slave. Enter in the device address and (if present) command code and data, and the GUI will send them to the adpater and the adapter will master the given transaction to the given slave device (if present). The status of the operation is shown to the right. ACK indicates that the transaction is successful. In the case of a read operation, the data should be shown. NACK indicates a failure of some type (no response, invalid response) and the data is not valid. 

For more information on this GUI, see the User's Guide for the USB Interface Adapter at the following URL: "http://focus.ti.com/lit/ml/sllu093/sllu093.pdf".

PMBus Reference GUI
===================

This GUI is another example of a low-level GUI that can be used with the adapter. This can be used to generate PMBus commands to a slave and show the low-level response of those commands.

Once you launch the GUI, you must create a new configuration file by selecting File->New Configuration. You then must select the PMBus address and give the power supply a name (optional). You then select which PMBus commands the device supports. Once you have done that, the file is loaded and you can do reads and writes of each of the PMBus commands the device supports. Put a check box next to the command that you wish to read and/or write and select the refresh button to read and the apply button to write.

This example GUI is not documented and is a reference example.

USB Interface Adapter Driver.zip
================================

This zip file contains a Microsoft .NET DLL that contains the low-level access routines that one needs to access the USB interface adapter. This is a .NET and DLL and - as such - can be referenced by any .NET language in a solution (C#, VB.NET, etc.). It was written using MS Visual Studio 2005. To include it in your project, add the DLL as a reference for the project in a solution that access it. When you browse through the DLL, you will see all the functions available to your program.

At this point, there is not any further documentation on this library.




