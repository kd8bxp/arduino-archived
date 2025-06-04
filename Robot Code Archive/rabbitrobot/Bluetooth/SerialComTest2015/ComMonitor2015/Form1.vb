Imports System
Imports System.IO
Imports System.IO.Ports
Imports System.Threading
Imports System.Text


Public Class Form1

    Dim buffer As String
    Delegate Sub myMethodDelegate(ByVal [text] As String)
    ' Dim bD1 As New myMethodDelegate(AddressOf process)
    Dim WithEvents SerialPort As New IO.Ports.SerialPort

    Private Sub Form1_Disposed(ByVal sender As Object, ByVal e As System.EventArgs) Handles Me.Disposed
        If SerialPort.IsOpen() Then
            SerialPort.Close()
        End If
    End Sub

    Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        GetSerialPortNames()

        SetBaudRate(9600)
        SetDataBits(8)
        SetParity(0)
        SetStopBits(1)
        SetHandshake(0)


        UpdateStatusStripCom()

        ' start in the middle screen
        Me.Top = (Screen.PrimaryScreen.WorkingArea.Height / 2) - (Me.Height / 2)
        Me.Left = (Screen.PrimaryScreen.WorkingArea.Width / 2) - (Me.Width / 2)

    End Sub

    Private Sub UpdateStatusStripCom()
        Static MyPortStatus As String
        Static MyPortParity As String
        Static MyPortFlow As String
        Static MyPortStopBits As String

        If SerialPort.IsOpen() Then
            MyPortStatus = "Connected"
        Else
            MyPortStatus = "Not Connected"
        End If

        Select Case SerialPort.Parity
            Case 0
                MyPortParity = "None"
            Case 1
                MyPortParity = "Odd"
            Case 2
                MyPortParity = "Even"
            Case 3
                MyPortParity = "Mark"
            Case 4
                MyPortParity = "Space"
            Case Else
                MyPortParity = "Error"
        End Select

        Select Case SerialPort.Handshake
            Case Handshake.None ' 0
                MyPortFlow = "No Handshake"
            Case Handshake.XOnXOff ' 1
                MyPortFlow = "XOn/XOff"
            Case Handshake.RequestToSend ' 2
                MyPortFlow = "RTS"
            Case Handshake.RequestToSendXOnXOff ' 3
                MyPortFlow = "RTS XOn/XOff"
            Case Else
                MyPortFlow = "Error"
        End Select

        Select Case SerialPort.StopBits
            Case 0
                MyPortStopBits = "0"
            Case 1
                MyPortStopBits = "1"
            Case 2
                MyPortStopBits = "2"
            Case 3
                MyPortStopBits = "1.5"
            Case Else
                MyPortStopBits = "Error"
        End Select

        ToolStripStatusLabelCom.Text = MyPortStatus & ", " & SerialPort.PortName _
            & ": " & SerialPort.BaudRate _
            & ", " & SerialPort.DataBits _
            & ", " & MyPortParity _
            & ", " & MyPortStopBits _
            & ", " & MyPortFlow   ' handshake method.. data flow control

        '            & ": " & SerialPort.Encoding

        ' baud rates: 75,110,134,150,300,600,1200,1800,2400,7200,9600,19200,38400,57600,115200,128000
        ' CTS, CD, Ring, DTR, RTS
        ' Parity: even, odd, none, mark, space
        ' flow control: X on/off, Hardware, none  .. handshake
        ' Stop bits: 1, 1.5, 2
        ' data bits: 4,5,6,7,8

    End Sub

    Sub GetSerialPortNames()

        lstPorts.Items.Clear()

        For Each sp As String In My.Computer.Ports.SerialPortNames
            lstPorts.Items.Add(sp)
            If sp <> "" Then
                SerialPort.PortName = sp
                lstPorts.SelectedIndex = lstPorts.Items.Count - 1
            End If
        Next
        UpdateStatusStripCom()

    End Sub

    Private Sub lstPorts_SelectedIndexChanged(sender As Object, e As EventArgs) Handles lstPorts.SelectedIndexChanged

        If lstPorts.SelectedItem.ToString <> "" Then
            SerialPort.PortName = lstPorts.SelectedItem.ToString
        End If


    End Sub

    Sub SendSerialData(ByVal Port As String, ByVal data As String)
        If (SerialPort.IsOpen) Then
            SerialPort.Write(data)
            ' MsgBox("Sent data to serial port: " & SerialPort.PortName, vbInformation, data)
        Else
            MsgBox("Not connected to Port.")
        End If
    End Sub

    Sub process(ByVal myString As String)
        buffer = buffer + myString
        Dim str As String
        str = buffer
        If InStr(str, "|") Then
            Dim words As String() = str.Split(New Char() {"|"})
            buffer = ""
            Dim word As String
            For Each word In words
                If (word.Length > 0) Then
                    lstConsole.Items.Add(word)
                End If
            Next
        End If

    End Sub



    Private Sub SerialPort_DataReceived(ByVal sender As Object, ByVal e As System.IO.Ports.SerialDataReceivedEventArgs) Handles SerialPort.DataReceived
        Dim strCaught As String
        strCaught = ""
        ' Me.ReceivedData.Text = " NEW TEXT INFO RECEIVED' "
        Static serialchar As String
        serialchar = ""

        If Me.InvokeRequired Then
            Me.Invoke(Sub() SerialPort_DataReceived(sender, e))
            Return
        End If

        ' must read the serial port information after the proper thread is invoked.
        'strCaught = SerialPort.ReadExisting()



        ' used to help identify the separated characters in the incoming stream
        Do While SerialPort.BytesToRead <> 0
            serialchar = SerialPort.ReadChar()
            If Val(serialchar) < 32 Then
                strCaught = strCaught & "<" & Hex(serialchar) & ">"
            Else
                strCaught = strCaught & Chr(serialchar)
            End If
        Loop

        Me.ReceivedData.Text = ReceivedData.Text & strCaught

        ' move to the bottom of the scrolled text box??

        'Invoke(bD1, str)

        ' definite problem here with multi-threading application.  
        ' Need to send the received data across to thread that constructed the textbox or other control.
        ' The difference is that Invoke() highly increase the odds your program will hang with a deadlock. 
        ' Always use BeginInvoke() instead. Particularly troublesome with the SerialPort class, 
        ' the kind of class where you have to use BeginInvoke in the DataReceived event handler 
        ' since it runs on a worker thread and thus cannot directly update controls. 
        ' Using Invoke() causes deadlock when you call the SerialPort.Close() method. –  
        ' problem solved by using the If me.invokerequired .... much better than just invoking all the time or not at all.

    End Sub


    Private Sub ButtonCMD1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonCMD1.Click
        ' get the custom commands from the INI file
        If SerialPort.IsOpen Then
            SerialPort.Write("CMD1")
        End If
    End Sub

    Private Sub ButtonCMD2_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonCMD2.Click
        If SerialPort.IsOpen Then
            SerialPort.Write("CMD2")
        End If
    End Sub
    Private Sub ButtonCMD3_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonCMD3.Click
        If SerialPort.IsOpen Then
            SerialPort.Write("CMD3")
        End If
    End Sub
    Private Sub ButtonCMD4_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonCMD4.Click
        If SerialPort.IsOpen Then
            SerialPort.Write("CMD4")
        End If
    End Sub
    Private Sub ButtonCMDCRLF_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonCMDCRLF.Click
        If SerialPort.IsOpen Then
            '    SerialPort.Write(vbCrLf) 
            ' vbCrLf is the easy way to do this item, but we want this to be an example of any special character code.

            Static test1 As String
            test1 = Chr(Val("&h0A"))
            test1 = test1 & Chr(Val("&h0D"))
            SerialPort.Write(test1)
            'SerialPort.Write(Chr(Val("&h24")) 
            '& Chr(Val("&hA")) & Chr(Val("&hD")))

        End If
    End Sub

    Private Sub OpenToolStripMenuItem1_Click(sender As Object, e As EventArgs) Handles OpenToolStripMenuItem1.Click
        ConnectSP()
    End Sub

    Private Sub ButtonConnectSP_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonConnectSP.Click
        ConnectSP()
    End Sub

    Sub ConnectSP()
        ' toggles the serial port open / closed  
        ' .. initiates the settings prior to opening the port
        ' baud rates: 75,110,134,150,300,600,1200,1800,2400,7200,9600,19200,38400,57600,115200,128000
        ' CTS, CD, Ring, DTR, RTS
        ' Parity: even, odd, none, mark, space
        ' flow control: X on/off, Hardware, none
        ' Stop bits: 1, 1.5, 2
        ' data bits: 4,5,6,7,8


        If lstPorts.SelectedIndex <> -1 Then
            Try
                If SerialPort.IsOpen Then
                    ' do nothing
                Else
                    SerialPort.PortName = lstPorts.SelectedItem.ToString
                End If

                ' MsgBox("Checking status on serial port: " & SerialPort.PortName, vbInformation, "Serial Port")
                If SerialPort.IsOpen Then
                    SerialPort.Close()
                    ButtonConnectSP.Text = "Connect"
                    OpenToolStripMenuItem1.Text = "&Connect"
                    SelectPortToolStripMenuItem.Enabled = True
                    BaudRateToolStripMenuItem.Enabled = True
                    DataBitsToolStripMenuItem.Enabled = True
                    ToolStripMenuItemParity.Enabled = True
                    StopBitsToolStripMenuItem.Enabled = True
                    HandshakeToolStripMenuItem.Enabled = True
                    EncodingToolStripMenuItem.Enabled = True
                    ' leaving the port Open/Close menu item alone
                    RefreshPortOptionsToolStripMenuItem.Enabled = True

                Else

                    If SerialPort.IsOpen Then
                        SerialPort.Close() ' closes the serial port so that we can initialize the settings
                    End If

                    ' initialize settings according to our selections

                    SerialPort.PortName = lstPorts.SelectedItem.ToString ' "COM4" ?
                    SerialPort.BaudRate = 9600
                    SerialPort.DataBits = 8
                    SerialPort.Parity = Parity.None
                    SerialPort.StopBits = StopBits.One
                    SerialPort.Handshake = Handshake.None
                    SerialPort.Encoding = System.Text.Encoding.Default
                    SerialPort.DtrEnable = False
                    SerialPort.RtsEnable = False
                    SerialPort.NewLine = vbCrLf
                    SerialPort.ReadTimeout = 10000
                    SerialPort.Open()
                    ButtonConnectSP.Text = "Disconnect"
                    OpenToolStripMenuItem1.Text = "&Disconnect"
                    ' disable the settings menu items
                    SelectPortToolStripMenuItem.Enabled = False
                    BaudRateToolStripMenuItem.Enabled = False
                    DataBitsToolStripMenuItem.Enabled = False
                    ToolStripMenuItemParity.Enabled = False
                    StopBitsToolStripMenuItem.Enabled = False
                    HandshakeToolStripMenuItem.Enabled = False
                    EncodingToolStripMenuItem.Enabled = False
                    ' leaving the port Open/Close menu item alone
                    RefreshPortOptionsToolStripMenuItem.Enabled = False

                End If
            Catch ex As Exception
                MsgBox(ex.Message)
            End Try
        Else
            MsgBox("Please choose a serial port", vbInformation, "Serial Port")
        End If

        UpdateStatusStripCom()

    End Sub

    Private Sub ButtonClear_Click(sender As Object, e As EventArgs) Handles ButtonClear.Click
        ReceivedData.Text = ""
    End Sub

    Private Sub ButtonSave_Click(sender As Object, e As EventArgs) Handles ButtonSave.Click
        ' save the ReceivedData buffer to a text file


        Static filenum As String
        Static path As String
        filenum = 0
        path = Application.StartupPath & "\ReceivedDataBuffer" & Str(filenum) & ".txt"

        Do While File.Exists(path) = True
            filenum = filenum + 1
            path = Application.StartupPath & "\ReceivedDataBuffer" & Str(filenum) & ".txt"
        Loop

        ' This text is added only once to the file.
        If File.Exists(path) = False Then

            ' Create a file to write to.
            Static createText As String = "Com Monitor - Rabbit" + Environment.NewLine
            File.WriteAllText(path, createText & vbCrLf & ReceivedData.Text)
        End If

        ' This text is always added, making the file longer over time
        ' if it is not deleted.
        'Dim appendText As String = "This is extra text" + Environment.NewLine
        'File.AppendAllText(path, appendText)

        ' Open the file to read from.
        'Dim readText As String = File.ReadAllText(path)
        'Console.WriteLine(readText)

    End Sub

    Private Sub AboutAuthorToolStripMenuItem_Click(sender As Object, e As EventArgs) Handles AboutRabbitToolStripMenuItem.Click
        MsgBox("Raymond Scott," & vbCrLf _
               & "Robotics Hobbyist," & vbCrLf _
               & "Monroe, Ohio", _
               vbInformation, "About the Author")
    End Sub

    Private Sub AboutToolStripMenuItem_Click(sender As Object, e As EventArgs) Handles AboutToolStripMenuItem.Click
        MsgBox("For diagnostics and development with serial communication port devices." & vbCrLf _
       & "Enter Hex data or simple text into the fields and send out the Com Port." & vbCrLf _
       & "Watch the data coming back into computer's Com Port.", _
       vbInformation, "About Com Monitor Rabbit")
    End Sub

    Private Sub RefreshPortOptionsToolStripMenuItem_Click(sender As Object, e As EventArgs) Handles RefreshPortOptionsToolStripMenuItem.Click
        ' call back to the serial port control to see what com ports are available now.
        GetSerialPortNames()
    End Sub

    Private Sub Button_SendHex1_Click(sender As Object, e As EventArgs) Handles Button_SendHex1.Click
        If SerialPort.IsOpen Then
            Static SerialPortString As String
            SerialPortString = ""
            If TextBox_Hex1_1.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBox_Hex1_1.Text))
            End If
            If TextBox_Hex1_2.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBox_Hex1_2.Text))
            End If
            If TextBox_Hex1_3.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBox_Hex1_3.Text))
            End If
            If TextBox_Hex1_4.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBox_Hex1_4.Text))
            End If
            If TextBox_Hex1_5.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBox_Hex1_5.Text))
            End If
            If TextBox_Hex1_6.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBox_Hex1_6.Text))
            End If
            If TextBox_Hex1_7.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBox_Hex1_7.Text))
            End If
            If TextBox_Hex1_8.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBox_Hex1_8.Text))
            End If
            SerialPort.Write(SerialPortString)
        End If
    End Sub

    Private Sub Button_SendHex2_Click(sender As Object, e As EventArgs) Handles Button_SendHex2.Click
        If SerialPort.IsOpen Then
            Static SerialPortString As String
            SerialPortString = ""
            If TextBox_Hex2_1.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBox_Hex2_1.Text))
            End If
            If TextBox_Hex2_2.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBox_Hex2_2.Text))
            End If
            If TextBox_Hex2_3.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBox_Hex2_3.Text))
            End If
            If TextBox_Hex2_4.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBox_Hex2_4.Text))
            End If
            If TextBox_Hex2_5.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBox_Hex2_5.Text))
            End If
            If TextBox_Hex2_6.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBox_Hex2_6.Text))
            End If
            If TextBox_Hex2_7.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBox_Hex2_7.Text))
            End If
            If TextBox_Hex2_8.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBox_Hex2_8.Text))
            End If
            SerialPort.Write(SerialPortString)
        End If
    End Sub

    Private Sub Button_SendHex3_Click(sender As Object, e As EventArgs) Handles Button_SendHex3.Click
        If SerialPort.IsOpen Then
            Static SerialPortString As String
            SerialPortString = ""
            If TextBox_Hex3_1.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBox_Hex3_1.Text))
            End If
            If TextBox_Hex3_2.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBox_Hex3_2.Text))
            End If
            If TextBox_Hex3_3.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBox_Hex3_3.Text))
            End If
            If TextBox_Hex3_4.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBox_Hex3_4.Text))
            End If
            If TextBox_Hex3_5.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBox_Hex3_5.Text))
            End If
            If TextBox_Hex3_6.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBox_Hex3_6.Text))
            End If
            If TextBox_Hex3_7.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBox_Hex3_7.Text))
            End If
            If TextBox_Hex3_8.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBox_Hex3_8.Text))
            End If
            SerialPort.Write(SerialPortString)
        End If
    End Sub

    Private Sub ButtonSendStr1_Click(sender As Object, e As EventArgs) Handles ButtonSendStr1.Click
        If SerialPort.IsOpen Then
            Static SerialPortString As String
            SerialPortString = ""
            If TextBoxSendStr1_1.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBoxSendStr1_1.Text))
            End If
            If TextBoxSendStr1_2.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBoxSendStr1_2.Text))
            End If
            If TextBoxSendStr1_3.Text <> "" Then
                SerialPortString = SerialPortString & TextBoxSendStr1_3.Text
            End If
            If TextBoxSendStr1_4.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBoxSendStr1_4.Text))
            End If
            If TextBoxSendStr1_5.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBoxSendStr1_5.Text))
            End If
            If TextBoxSendStr1_6.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBoxSendStr1_6.Text))
            End If
            SerialPort.Write(SerialPortString)
        End If
    End Sub

    Private Sub ButtonSendStr2_Click(sender As Object, e As EventArgs) Handles ButtonSendStr2.Click
        If SerialPort.IsOpen Then
            Static SerialPortString As String
            SerialPortString = ""
            If TextBoxSendStr2_1.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBoxSendStr2_1.Text))
            End If
            If TextBoxSendStr2_2.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBoxSendStr2_2.Text))
            End If
            If TextBoxSendStr2_3.Text <> "" Then
                SerialPortString = SerialPortString & TextBoxSendStr2_3.Text
            End If
            If TextBoxSendStr2_4.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBoxSendStr2_4.Text))
            End If
            If TextBoxSendStr2_5.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBoxSendStr2_5.Text))
            End If
            If TextBoxSendStr2_6.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBoxSendStr2_6.Text))
            End If
            SerialPort.Write(SerialPortString)
        End If
    End Sub

    Private Sub ButtonSendStr3_Click(sender As Object, e As EventArgs) Handles ButtonSendStr3.Click
        If SerialPort.IsOpen Then
            Static SerialPortString As String
            SerialPortString = ""
            If TextBoxSendStr3_1.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBoxSendStr3_1.Text))
            End If
            If TextBoxSendStr3_2.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBoxSendStr3_2.Text))
            End If
            If TextBoxSendStr3_3.Text <> "" Then
                SerialPortString = SerialPortString & TextBoxSendStr3_3.Text
            End If
            If TextBoxSendStr3_4.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBoxSendStr3_4.Text))
            End If
            If TextBoxSendStr3_5.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBoxSendStr3_5.Text))
            End If
            If TextBoxSendStr3_6.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBoxSendStr3_6.Text))
            End If
            SerialPort.Write(SerialPortString)
        End If
    End Sub

    Private Sub ButtonSendStr4_Click(sender As Object, e As EventArgs) Handles ButtonSendStr4.Click
        If SerialPort.IsOpen Then
            Static SerialPortString As String
            SerialPortString = ""
            If TextBoxSendStr4_1.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBoxSendStr4_1.Text))
            End If
            If TextBoxSendStr4_2.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBoxSendStr4_2.Text))
            End If
            If TextBoxSendStr4_3.Text <> "" Then
                SerialPortString = SerialPortString & TextBoxSendStr4_3.Text
            End If
            If TextBoxSendStr4_4.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBoxSendStr4_4.Text))
            End If
            If TextBoxSendStr4_5.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBoxSendStr4_5.Text))
            End If
            If TextBoxSendStr4_6.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBoxSendStr4_6.Text))
            End If
            SerialPort.Write(SerialPortString)
        End If
    End Sub

    Private Sub ButtonSendStr5_Click(sender As Object, e As EventArgs) Handles ButtonSendStr5.Click
        If SerialPort.IsOpen Then
            Static SerialPortString As String
            SerialPortString = ""
            If TextBoxSendStr5_1.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBoxSendStr5_1.Text))
            End If
            If TextBoxSendStr5_2.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBoxSendStr5_2.Text))
            End If
            If TextBoxSendStr5_3.Text <> "" Then
                SerialPortString = SerialPortString & TextBoxSendStr5_3.Text
            End If
            If TextBoxSendStr5_4.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBoxSendStr5_4.Text))
            End If
            If TextBoxSendStr5_5.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBoxSendStr5_5.Text))
            End If
            If TextBoxSendStr5_6.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBoxSendStr5_6.Text))
            End If
            SerialPort.Write(SerialPortString)
        End If
    End Sub

    Private Sub ButtonSendStr6_Click(sender As Object, e As EventArgs) Handles ButtonSendStr6.Click
        If SerialPort.IsOpen Then
            Static SerialPortString As String
            SerialPortString = ""
            If TextBoxSendStr6_1.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBoxSendStr6_1.Text))
            End If
            If TextBoxSendStr6_2.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBoxSendStr6_2.Text))
            End If
            If TextBoxSendStr6_3.Text <> "" Then
                SerialPortString = SerialPortString & TextBoxSendStr6_3.Text
            End If
            If TextBoxSendStr6_4.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBoxSendStr6_4.Text))
            End If
            If TextBoxSendStr6_5.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBoxSendStr6_5.Text))
            End If
            If TextBoxSendStr6_6.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBoxSendStr6_6.Text))
            End If
            SerialPort.Write(SerialPortString)
        End If
    End Sub

    Private Sub ButtonSendStr7_Click(sender As Object, e As EventArgs) Handles ButtonSendStr7.Click
        If SerialPort.IsOpen Then
            Static SerialPortString As String
            SerialPortString = ""
            If TextBoxSendStr7_1.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBoxSendStr7_1.Text))
            End If
            If TextBoxSendStr7_2.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBoxSendStr7_2.Text))
            End If
            If TextBoxSendStr7_3.Text <> "" Then
                SerialPortString = SerialPortString & TextBoxSendStr7_3.Text
            End If
            If TextBoxSendStr7_4.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBoxSendStr7_4.Text))
            End If
            If TextBoxSendStr7_5.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBoxSendStr7_5.Text))
            End If
            If TextBoxSendStr7_6.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBoxSendStr7_6.Text))
            End If
            SerialPort.Write(SerialPortString)
        End If
    End Sub

    Private Sub ButtonSendStr8_Click(sender As Object, e As EventArgs) Handles ButtonSendStr8.Click
        If SerialPort.IsOpen Then
            Static SerialPortString As String
            SerialPortString = ""
            If TextBoxSendStr8_1.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBoxSendStr8_1.Text))
            End If
            If TextBoxSendStr8_2.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBoxSendStr8_2.Text))
            End If
            If TextBoxSendStr8_3.Text <> "" Then
                SerialPortString = SerialPortString & TextBoxSendStr8_3.Text
            End If
            If TextBoxSendStr8_4.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBoxSendStr8_4.Text))
            End If
            If TextBoxSendStr8_5.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBoxSendStr8_5.Text))
            End If
            If TextBoxSendStr8_6.Text <> "" Then
                SerialPortString = SerialPortString & Chr(Val("&h" & TextBoxSendStr8_6.Text))
            End If
            SerialPort.Write(SerialPortString)
        End If
    End Sub

    Private Sub OpenToolStripMenuItem_Click(sender As Object, e As EventArgs) Handles OpenToolStripMenuItem.Click
        ' open configuration file
        Dim fd As OpenFileDialog = New OpenFileDialog()
        Dim strFileName As String

        fd.Title = "Open File Dialog"
        fd.InitialDirectory = "C:\"
        fd.Filter = "All files (*.*)|*.*|All files (*.*)|*.*"
        fd.FilterIndex = 2
        fd.RestoreDirectory = True

        If fd.ShowDialog() = DialogResult.OK Then
            strFileName = fd.FileName
            ' retain the path string.. without the filename attached ... for next time the user wants to open a file.. or save a file.

        End If
    End Sub



    Private Sub ToolStripMenuItemBaud75_Click(sender As Object, e As EventArgs) Handles ToolStripMenuItemBaud75.Click
        ' set the serial port baud rate
        SetBaudRate(75)
    End Sub

    Private Sub ToolStripMenuItemBaud110_Click(sender As Object, e As EventArgs) Handles ToolStripMenuItemBaud110.Click
        ' set the serial port baud rate
        SetBaudRate(110)
    End Sub

    Private Sub ToolStripMenuItemBaud134_Click(sender As Object, e As EventArgs) Handles ToolStripMenuItemBaud134.Click
        ' set the serial port baud rate
        SetBaudRate(134)
    End Sub

    Private Sub ToolStripMenuItemBaud150_Click(sender As Object, e As EventArgs) Handles ToolStripMenuItemBaud150.Click
        ' set the serial port baud rate
        SetBaudRate(150)
    End Sub

    Private Sub ToolStripMenuItemBaud300_Click(sender As Object, e As EventArgs) Handles ToolStripMenuItemBaud300.Click
        ' set the serial port baud rate
        SetBaudRate(300)
    End Sub

    Private Sub ToolStripMenuItemBaud600_Click(sender As Object, e As EventArgs) Handles ToolStripMenuItemBaud600.Click
        ' set the serial port baud rate
        SetBaudRate(600)
    End Sub

    Private Sub ToolStripMenuItemBaud1200_Click(sender As Object, e As EventArgs) Handles ToolStripMenuItemBaud1200.Click
        ' set the serial port baud rate
        SetBaudRate(1200)
    End Sub

    Private Sub ToolStripMenuItemBaud1800_Click(sender As Object, e As EventArgs) Handles ToolStripMenuItemBaud1800.Click
        ' set the serial port baud rate
        SetBaudRate(1800)
    End Sub

    Private Sub ToolStripMenuItemBaud2400_Click(sender As Object, e As EventArgs) Handles ToolStripMenuItemBaud2400.Click
        ' set the serial port baud rate
        SetBaudRate(2400)
    End Sub

    Private Sub ToolStripMenuItemBaud4800_Click(sender As Object, e As EventArgs) Handles ToolStripMenuItemBaud4800.Click
        ' set the serial port baud rate
        SetBaudRate(4800)
    End Sub

    Private Sub ToolStripMenuItemBaud7200_Click(sender As Object, e As EventArgs) Handles ToolStripMenuItemBaud7200.Click
        ' set the serial port baud rate
        SetBaudRate(7200)
    End Sub

    Private Sub ToolStripMenuItemBaud9600_Click(sender As Object, e As EventArgs) Handles ToolStripMenuItemBaud9600.Click
        ' set the serial port baud rate
        SetBaudRate(9600)
    End Sub

    Private Sub ToolStripMenuItemBaud19200_Click(sender As Object, e As EventArgs) Handles ToolStripMenuItemBaud19200.Click
        ' set the serial port baud rate
        SetBaudRate(19200)
    End Sub

    Private Sub ToolStripMenuItemBaud38400_Click(sender As Object, e As EventArgs) Handles ToolStripMenuItemBaud38400.Click
        ' set the serial port baud rate
        SetBaudRate(38400)
    End Sub

    Private Sub ToolStripMenuItemBaud57600_Click(sender As Object, e As EventArgs) Handles ToolStripMenuItemBaud57600.Click
        ' set the serial port baud rate
        SetBaudRate(57600)
    End Sub

    Private Sub ToolStripMenuItemBaud115200_Click(sender As Object, e As EventArgs) Handles ToolStripMenuItemBaud115200.Click
        ' set the serial port baud rate
        SetBaudRate(115200)
    End Sub


    Private Sub ToolStripMenuItemBaud128000_Click(sender As Object, e As EventArgs) Handles ToolStripMenuItemBaud128000.Click
        ' set the serial port baud rate
        SetBaudRate(128000)
    End Sub

    Sub SetBaudRate(Rate As Integer)
        Static oldBaudRate As Integer

        oldBaudRate = SerialPort.BaudRate
        SerialPort.BaudRate = Rate

        ToolStripMenuItemBaud75.Checked = False
        ToolStripMenuItemBaud110.Checked = False
        ToolStripMenuItemBaud134.Checked = False
        ToolStripMenuItemBaud150.Checked = False
        ToolStripMenuItemBaud300.Checked = False
        ToolStripMenuItemBaud600.Checked = False
        ToolStripMenuItemBaud1200.Checked = False
        ToolStripMenuItemBaud1800.Checked = False
        ToolStripMenuItemBaud2400.Checked = False
        ToolStripMenuItemBaud7200.Checked = False
        ToolStripMenuItemBaud9600.Checked = False
        ToolStripMenuItemBaud19200.Checked = False
        ToolStripMenuItemBaud38400.Checked = False
        ToolStripMenuItemBaud57600.Checked = False
        ToolStripMenuItemBaud115200.Checked = False
        ToolStripMenuItemBaud128000.Checked = False

        Select Case Rate
            Case 75
                ToolStripMenuItemBaud75.Checked = True
            Case 110
                ToolStripMenuItemBaud110.Checked = True
            Case 134
                ToolStripMenuItemBaud134.Checked = True
            Case 150
                ToolStripMenuItemBaud150.Checked = True
            Case 300
                ToolStripMenuItemBaud300.Checked = True
            Case 600
                ToolStripMenuItemBaud600.Checked = True
            Case 1200
                ToolStripMenuItemBaud1200.Checked = True
            Case 1800
                ToolStripMenuItemBaud1800.Checked = True
            Case 2400
                ToolStripMenuItemBaud2400.Checked = True
            Case 7200
                ToolStripMenuItemBaud7200.Checked = True
            Case 9600
                ToolStripMenuItemBaud9600.Checked = True
            Case 19200
                ToolStripMenuItemBaud19200.Checked = True
            Case 38400
                ToolStripMenuItemBaud38400.Checked = True
            Case 57600
                ToolStripMenuItemBaud57600.Checked = True
            Case 115200
                ToolStripMenuItemBaud115200.Checked = True
            Case 128000
                ToolStripMenuItemBaud128000.Checked = True
            Case Else
                MsgBox("Error while trying to set the Baud rate.")
        End Select

        UpdateStatusStripCom()

    End Sub

    Sub SetDataBits(DataBits As Integer)
        Static oldDataBits As Integer

        oldDataBits = SerialPort.DataBits
        SerialPort.DataBits = DataBits

        ToolStripMenuItemData4.Checked = False
        ToolStripMenuItemData5.Checked = False
        ToolStripMenuItemData6.Checked = False
        ToolStripMenuItemData7.Checked = False
        ToolStripMenuItemData8.Checked = False

        Select Case DataBits
            Case 4
                ToolStripMenuItemData4.Checked = True
            Case 5
                ToolStripMenuItemData5.Checked = True
            Case 6
                ToolStripMenuItemData6.Checked = True
            Case 7
                ToolStripMenuItemData7.Checked = True
            Case 8
                ToolStripMenuItemData8.Checked = True
            Case Else
                MsgBox("Error while trying to set the Data Bits.")
        End Select

        UpdateStatusStripCom()
    End Sub

    Private Sub ToolStripMenuItemData4_Click(sender As Object, e As EventArgs) Handles ToolStripMenuItemData4.Click
        SetDataBits(4)
    End Sub

    Private Sub ToolStripMenuItemData5_Click(sender As Object, e As EventArgs) Handles ToolStripMenuItemData5.Click
        SetDataBits(5)
    End Sub

    Private Sub ToolStripMenuItemData6_Click(sender As Object, e As EventArgs) Handles ToolStripMenuItemData6.Click
        SetDataBits(6)
    End Sub

    Private Sub ToolStripMenuItemData7_Click(sender As Object, e As EventArgs) Handles ToolStripMenuItemData7.Click
        SetDataBits(7)
    End Sub

    Private Sub ToolStripMenuItemData8_Click(sender As Object, e As EventArgs) Handles ToolStripMenuItemData8.Click
        SetDataBits(8)
    End Sub


    Private Sub SelectPortToolStripMenuItem_Click(sender As Object, e As EventArgs) Handles SelectPortToolStripMenuItem.Click

    End Sub

    Sub SetParity(ParityVal As Integer)
        Static oldParity As Integer

        oldParity = SerialPort.Parity
        SerialPort.Parity = ParityVal

        ToolStripMenuItemParityNone.Checked = False
        ToolStripMenuItemParityOdd.Checked = False
        ToolStripMenuItemParityEven.Checked = False
        ToolStripMenuItemParityMark.Checked = False
        ToolStripMenuItemParitySpace.Checked = False

        Select Case ParityVal
            Case 0
                ToolStripMenuItemParityNone.Checked = True
            Case 1
                ToolStripMenuItemParityOdd.Checked = True
            Case 2
                ToolStripMenuItemParityEven.Checked = True
            Case 3
                ToolStripMenuItemParityMark.Checked = True
            Case 4
                ToolStripMenuItemParitySpace.Checked = True
            Case Else
                MsgBox("Error while trying to set the Com Port Parity.")
        End Select

        UpdateStatusStripCom()
    End Sub

    Private Sub ToolStripMenuItemParityNone_Click(sender As Object, e As EventArgs) Handles ToolStripMenuItemParityNone.Click
        SetParity(0)
    End Sub

    Private Sub ToolStripMenuItemParityOdd_Click(sender As Object, e As EventArgs) Handles ToolStripMenuItemParityOdd.Click
        SetParity(1)
    End Sub

    Private Sub ToolStripMenuItemParityEven_Click(sender As Object, e As EventArgs) Handles ToolStripMenuItemParityEven.Click
        SetParity(2)
    End Sub

    Private Sub ToolStripMenuItemParityMark_Click(sender As Object, e As EventArgs) Handles ToolStripMenuItemParityMark.Click
        SetParity(3)
    End Sub

    Private Sub ToolStripMenuItemParitySpace_Click(sender As Object, e As EventArgs) Handles ToolStripMenuItemParitySpace.Click
        SetParity(4)
    End Sub

    Sub SetStopBits(StopVal As Integer)

        Static oldStopVal As Integer

        oldStopVal = SerialPort.StopBits
        SerialPort.StopBits = StopVal

        ToolStripMenuItemStop0.Checked = False
        ToolStripMenuItemStop1.Checked = False
        ToolStripMenuItemStop15.Checked = False
        ToolStripMenuItemStop2.Checked = False


        Select Case StopVal
            Case 0
                ToolStripMenuItemStop0.Checked = True
            Case 1
                ToolStripMenuItemStop1.Checked = True
            Case 2
                ToolStripMenuItemStop2.Checked = True
            Case 3
                ToolStripMenuItemStop15.Checked = True

            Case Else
                MsgBox("Error while trying to set the Com Port Stop Bits.")
        End Select

        UpdateStatusStripCom()

    End Sub


    Private Sub ToolStripMenuItemStop0_Click(sender As Object, e As EventArgs) Handles ToolStripMenuItemStop0.Click
        SetStopBits(0)
    End Sub

    Private Sub ToolStripMenuItemStop1_Click(sender As Object, e As EventArgs) Handles ToolStripMenuItemStop1.Click
        SetStopBits(1)
    End Sub

    Private Sub ToolStripMenuItemStop15_Click(sender As Object, e As EventArgs) Handles ToolStripMenuItemStop15.Click
        SetStopBits(3)
    End Sub

    Private Sub ToolStripMenuItemStop2_Click(sender As Object, e As EventArgs) Handles ToolStripMenuItemStop2.Click
        SetStopBits(2)
    End Sub

    Sub SetHandshake(HandshakeVal As Integer)
        Static oldHandshakeVal As Integer

        oldHandshakeVal = SerialPort.Handshake
        SerialPort.Handshake = HandshakeVal

        ToolStripMenuItemHandshakeNone.Checked = False ' 0
        ToolStripMenuItemHandshakeXOn.Checked = False ' 1
        ToolStripMenuItemHandshakeRTS.Checked = False ' 2
        ToolStripMenuItemHandshakeRTSXOn.Checked = False ' 3


        Select Case HandshakeVal
            Case 0
                ToolStripMenuItemHandshakeNone.Checked = True
            Case 1
                ToolStripMenuItemHandshakeXOn.Checked = True
            Case 2
                ToolStripMenuItemHandshakeRTS.Checked = True
            Case 3
                ToolStripMenuItemHandshakeRTSXOn.Checked = True

            Case Else
                MsgBox("Error while trying to set the Com Port Handshake Flow Control.")
        End Select

        UpdateStatusStripCom()

    End Sub


End Class
