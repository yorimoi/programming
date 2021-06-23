Imports System.Reflection.Emit

Public Class MainForm

    ''' <summary>
    ''' MainForm上のキーダウンイベントを受け取る
    ''' </summary>
    Private Sub MainForm_KeyDown(sender As Object, e As KeyEventArgs) Handles MyBase.KeyDown
        Select Case e.KeyValue
            Case Keys.Back, Keys.Delete: ExpressionTextBox.Text = TrimEnd(ExpressionTextBox.Text)
            Case Keys.Enter: Calclate()
        End Select

        ' カーソル位置を最後に移動
        ExpressionTextBox.SelectionStart = ExpressionTextBox.Text.Length
    End Sub

    ''' <summary>
    ''' MainForm上のキープレスイベントを受け取る
    ''' </summary>
    Private Sub MainForm_KeyPress(sender As Object, e As KeyPressEventArgs) Handles MyBase.KeyPress
        Select Case e.KeyChar
            Case "0", "1" ,"2" ,"3" ,"4" ,"5" ,"6" ,"7" ,"8" ,"9" ,"(" ,")" ,"." ,"+" ,"-" ,"*" ,"/" : ExpressionTextBox.Text += e.KeyChar
            Case "=" : Calclate()
            Case Else: Return
        End Select

        ' カーソル位置を最後に移動
        ExpressionTextBox.SelectionStart = ExpressionTextBox.Text.Length
    End Sub

    ''' <summary>
    ''' ボタンクリックイベント
    ''' </summary>
    ''' <param name="sender"></param>
    ''' <param name="e"></param>
    Private Sub Button_Click(sender As Object, e As EventArgs) Handles ZeroButton.Click, TwoButton.Click, ThreeButton.Click, SlashButton.Click,
        SixButton.Click, SevenButton.Click, RightParenButton.Click, PlusButton.Click, OneButton.Click, NineButton.Click, MinusButton.Click,
        LeftParenButton.Click, FourButton.Click, FiveButton.Click, EqualButton.Click, EightButton.Click, DotButton.Click, DeleteButton.Click,
        ClearButton.Click, AsterriskButton.Click

        Select Case CType(sender, Button).Text
            Case "0","1","2","3","4","5","6","7","8","9",".","+","-","*","/","(",")": ExpressionTextBox.Text += CType(sender, Button).Text
            Case "=": Calclate()
            Case "DEL": ExpressionTextBox.Text = TrimEnd(ExpressionTextBox.Text)
            Case "CLS": ExpressionTextBox.Text = ""
            Case Else: Return ' Unreachable
        End Select
    End Sub

    ''' <summary>
    ''' 最後の1文字を削除した文字列を返す
    ''' </summary>
    ''' <param name="text"></param>
    ''' <returns></returns>
    Private Function TrimEnd(text As String) As String
        If String.IsNullOrEmpty(text) Then
            Return ""
        Else
            Return text.Substring(0, text.Length - 1)
        End If
    End Function

    ''' <summary>
    ''' 計算をして表示する
    ''' </summary>
    Private Sub Calclate()
        ' 式が入力されていないなら何もしない
        If Me.ExpressionTextBox.Text.Length = 0 Then
            Return
        End If

        ' トークナイズ
        Dim tokens As Token() = Token.Tokenize(Me.ExpressionTextBox.Text)
        Console.WriteLine(tokens.Length())
        ' ASTの構築
        Dim node As Ast = Ast.Generate(tokens)
        ' 計算
        Dim result As Integer = Evaluate(node)
        ' 結果を表示
        Me.AnswerTextBox.Text = result.ToString()
    End Sub

    ''' <summary>
    ''' ASTから計算し、結果を返す
    ''' </summary>
    ''' <param name="node">AST</param>
    ''' <returns>計算結果</returns>
    Private Shared Function Evaluate(node As Ast) As Integer
        Dim stack As New Stack(Of Integer)
        Dim dm As New DynamicMethod("exec", GetType(Integer), Nothing)
        Dim il As ILGenerator = dm.GetILGenerator()
        Try
            Express(il, node)
            il.Emit(OpCodes.Ret)
            Return dm.Invoke(il, Nothing)
        Catch ex As DivideByZeroException
            MessageBox.Show(ex.Message, Application.ProductName, MessageBoxButtons.OK, MessageBoxIcon.Error)
        Catch ex As Exception
            MessageBox.Show("エラーが発生しました", Application.ProductName, MessageBoxButtons.OK, MessageBoxIcon.Error)
        End Try
        Return 0
    End Function

    Private Shared Sub Express(ByRef il As ILGenerator, ByRef node As Ast)
        If node.Type = NodeType.Num Then
            il.Emit(OpCodes.Ldc_I4, node.Num)
            Return
        End If

        Express(il, node.LNode)
        Express(il, node.RNode)

        Select case node.Type
            Case NodeType.Add:
                il.Emit(OpCodes.Add)
            Case NodeType.Subt:
                il.Emit(OpCodes.Sub)
            Case NodeType.Mul:
                il.Emit(OpCodes.Mul)
            Case NodeType.Div:
                If node.RNode.Num = 0 Then
                    Throw New DivideByZeroException()
                End If
                il.Emit(OpCodes.Div)
        End Select
    End Sub
End Class
