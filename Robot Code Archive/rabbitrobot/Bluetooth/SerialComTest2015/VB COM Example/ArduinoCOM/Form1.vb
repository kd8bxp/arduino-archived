Imports System.IO
Imports System.IO.Ports
Imports System.Threading

Public Class Form1
    Dim buffer As String
    Delegate Sub myMethodDelegate(ByVal [text] As String)
    Dim bD1 As New myMethodDelegate(AddressOf process)
    Dim WithEvents SerialPort As New IO.Ports.SerialPort

    Private Sub Form1_Disposed(ByVal sender As Object, ByVal e As System.EventArgs) Handles Me.Disposed
        If SerialPort.IsOpen() Then
            SerialPort.Close()
        End If
    End Sub

    Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        GetSerialPortNames()
    End Sub

    Sub GetSerialPortNames()
        For Each sp As String In My.Computer.Ports.SerialPortNames
            lstPorts.Items.Add(sp)
        Next
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
        ' Me.ReceivedData.Text = " NEW TEXT INFO RECEIVED' "

        If Me.InvokeRequired Then
            Me.Invoke(Sub() SerialPort_DataReceived(sender, e))
            Return
        End If

        ' must read the serial port information after the proper thread is invoked.
        strCaught = SerialPort.ReadExisting()
        Me.ReceivedData.Text = ReceivedData.Text & strCaught

        'Invoke(bD1, str)

        ' definite problem here with multi-threading application.  
        ' Need to send the received data across to thread that constructed the textbox or other control.
        ' The difference is that Invoke() highly increase the odds your program will hang with a deadlock. 
        ' Always use BeginInvoke() instead. Particularly troublesome with the SerialPort class, 
        ' the kind of class where you have to use BeginInvoke in the DataReceived event handler 
        ' since it runs on a worker thread and thus cannot directly update controls. 
        ' Using Invoke() causes deadlock when you call the SerialPort.Close() method. –  

    End Sub

    Private Sub ButtonConnectSP_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonConnectSP.Click
        If lstPorts.SelectedIndex <> -1 Then
            Try
                If SerialPort.IsOpen Then
                Else
                    SerialPort.PortName = lstPorts.SelectedItem.ToString
                End If
                ' MsgBox("Checking status on serial port: " & SerialPort.PortName, vbInformation, "Serial Port")
                If SerialPort.IsOpen Then
                    SerialPort.Close()
                    ButtonConnectSP.Text = "Connect"
                Else
                    SerialPort.PortName = lstPorts.SelectedItem.ToString
                    SerialPort.BaudRate = 9600
                    SerialPort.DataBits = 8
                    SerialPort.Parity = Parity.None
                    SerialPort.StopBits = StopBits.One
                    SerialPort.Handshake = Handshake.None
                    SerialPort.Encoding = System.Text.Encoding.Default
                    SerialPort.Open()
                    ButtonConnectSP.Text = "Disconnect"
                End If
            Catch ex As Exception
                MsgBox(ex.Message)
            End Try
        Else
            MsgBox("Please choose a serial port", vbInformation, "Serial Port")
        End If
    End Sub

 
    Private Sub Button3_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonQuit.Click
        Application.Exit()
        If SerialPort.IsOpen Then
            SerialPort.Close()
        End If
    End Sub


    Private Sub ButtonQuit_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonQuit.Click
        Application.Exit()
        If SerialPort.IsOpen Then
            SerialPort.Close()
            'MsgBox("Serial Port was Open/Connected.  Now Closed.", vbInformation, "Serial Port")
        End If
    End Sub

    Private Sub Button1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button1.Click
        '' toggle light
        If (lstPorts.SelectedIndex <> -1) Then
            SendSerialData(lstPorts.SelectedItem.ToString, "Toggle Lights")
            ''SerialPort.
        End If
    End Sub

    Private Sub Button4_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button4.Click
        If (lstPorts.SelectedIndex <> -1) Then
            SendSerialData(lstPorts.SelectedItem.ToString, "Test")
        End If
    End Sub


    Private Sub ReceivedData_TextChanged(sender As Object, e As EventArgs) Handles ReceivedData.TextChanged

    End Sub
End Class