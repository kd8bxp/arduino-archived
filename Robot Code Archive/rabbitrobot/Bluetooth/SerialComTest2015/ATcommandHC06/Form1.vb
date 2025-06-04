Imports System.Threading

Public Class Form1
    Private readBuffer As String

    Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        Dim Portnames As String() = System.IO.Ports.SerialPort.GetPortNames
        If Portnames Is Nothing Then
            Dim i As Integer
            For i = 1 To 20 Step 1
                ComboBox1.Items.Add("COM" & i)
            Next i
        Else
            ComboBox1.Items.AddRange(Portnames)
        End If
    End Sub

    Private Sub Form1_FormClosed(ByVal sender As System.Object, ByVal e As System.Windows.Forms.FormClosedEventArgs) Handles MyBase.FormClosed
        If (SerialPort1.IsOpen = True) Then SerialPort1.Dispose()
    End Sub

    Private Sub Button1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button1.Click
        If (SerialPort1.IsOpen = True) Then
            SerialPort1.Dispose()
        Else
            SerialPort1.PortName = ComboBox1.Text
            SerialPort1.BaudRate = CInt(ComboBox2.Text)
            SerialPort1.Parity = IO.Ports.Parity.None '
            If ComboBox3.Text = "Even" Then SerialPort1.Parity = IO.Ports.Parity.Even
            If ComboBox3.Text = "Odd" Then SerialPort1.Parity = IO.Ports.Parity.Odd
            SerialPort1.DataBits = CInt(ComboBox4.Text)
            SerialPort1.StopBits = IO.Ports.StopBits.One
            If ComboBox5.Text = "Two" Then SerialPort1.StopBits = IO.Ports.StopBits.Two
            Try
                SerialPort1.Open()
            Catch ex As Exception
                Me.Text = ex.Message
            End Try
        End If

    End Sub

    Private Sub SerialPort1_DataReceived(ByVal sender As System.Object, ByVal e As System.IO.Ports.SerialDataReceivedEventArgs) Handles SerialPort1.DataReceived
        If (SerialPort1.IsOpen = True) Then
            Try
                readBuffer = SerialPort1.ReadExisting
                Me.Invoke(New EventHandler(AddressOf DoUpdate))
            Catch ex As Exception
                MsgBox("read " & ex.Message)
            End Try
        End If
    End Sub

    Public Sub DoUpdate(ByVal sender As Object, ByVal e As System.EventArgs)
        RichTextBox1.Text = RichTextBox1.Text & readBuffer
    End Sub

    Private Sub Button2_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button2.Click
        RichTextBox1.Clear()
        If (SerialPort1.IsOpen = True) Then
            Try
                Dim send As String
                send = UCase(TextBox1.Text)
                SerialPort1.Write(send)
            Catch ex As Exception
                MsgBox("send " & ex.Message)
            End Try
        End If
    End Sub

    Private Sub Timer1_Tick(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Timer1.Tick
        If (SerialPort1.IsOpen = True) Then
            Label1.Visible = True
            Label2.Visible = True
            TextBox1.Visible = True
            RichTextBox1.Visible = True
            Button2.Visible = True
        Else
            Label1.Visible = False
            Label2.Visible = False
            TextBox1.Visible = False
            RichTextBox1.Visible = False
            Button2.Visible = False
        End If
    End Sub
End Class
