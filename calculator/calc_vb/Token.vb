''' <summary>
''' トークンの種類
''' </summary>
Public Enum TokenType
    Invalid   ' 無効なトークン
    Plus      ' +
    Minus     ' -
    Asterisk  ' *
    Slash     ' /
    Num       ' 整数
    LParen    ' (
    RParen    ' )
    Dot       ' .
    EOF       ' End Of Tokens
End Enum

''' <summary>
''' トークンの定義
''' </summary>
Public Class Token

    ''' <summary>
    ''' トークンの種類
    ''' </summary>
    Public Property Type As TokenType

    Private _num As Integer
    ''' <summary>
    ''' トークンが整数だった場合に使用
    ''' </summary>
    ''' <returns>整数</returns>
    ''' <exception cref="InvalidOperationException">トークンが「Num」ではない</exception>
    Public Property Num As Integer
        Get
            ' トークンが「Num」なら整数を返す
            If Me.Type <> TokenType.Invalid Then
                Return Me._num
            End If
            Throw New InvalidOperationException("トークンが整数ではありません")
        End Get
        Set(value As Integer)
            Me._num = value
        End Set
    End Property

    Private Sub New()
        Throw New NotImplementedException("Unreachable")
    End Sub

    ''' <summary>
    ''' コンストラクタ
    ''' </summary>
    ''' <param name="type"></param>
    Private Sub New(type As TokenType)
        Me.Type = type
    End Sub

    ''' <summary>
    ''' 整数用コンストラクタ
    ''' </summary>
    ''' <param name="num"></param>
    Private Sub New(num As Integer)
        Me.Type = TokenType.Num
        Me.Num = num
    End Sub

    Public Shared Function Tokenize(input As String) As Token()
        Dim tokens As List(Of Token) = New List(Of Token)
        Dim index As Integer = 0

        While index < input.Length
            Select Case input(index)
                Case "+"c: tokens.Add(New Token(TokenType.Plus))
                Case "-"c: tokens.Add(New Token(TokenType.Minus))
                Case "*"c: tokens.Add(New Token(TokenType.Asterisk))
                Case "/"c: tokens.Add(New Token(TokenType.Slash))
                Case "("c: tokens.Add(New Token(TokenType.LParen))
                Case ")"c: tokens.Add(New Token(TokenType.RParen))
                Case "."c: Throw New NotImplementedException()
                Case Else
                    If Char.IsNumber(input(index)) Then
                        Dim num As Integer = Integer.Parse(input(index))
                        While index+1 < input.Length AndAlso Char.IsNumber(input(index + 1))
                            num = num * 10 + Integer.Parse(input(index + 1))
                            index += 1
                        End While
                        tokens.Add(New Token(num))
                    Else
                        tokens.Add(New Token(TokenType.Invalid))
                    End If
            End Select

            index += 1
        End While

        tokens.Add(New Token(TokenType.EOF))
        Return tokens.ToArray()
    End Function
End Class

''' <summary>
''' Tokenのイテレータ
''' </summary>
Public Class TokenIter
    Private Tokens As Token()
    Private Index As Integer = 0

    ''' <summary>
    ''' コンストラクタ
    ''' </summary>
    ''' <param name="tokens">トークン列</param>
    Public Sub New(tokens As Token())
        Me.Tokens = tokens
    End Sub

    ''' <summary>
    ''' トークン列を消費
    ''' </summary>
    Public Sub Consume()
        Me.Index += 1
    End Sub

    ''' <summary>
    ''' 期待するトークンの種類であることを保証する
    ''' </summary>
    Public Sub Expect(type As TokenType)
        If Me.Tokens(Me.Index).Type <> type Then
            Throw New InvalidOperationException($"期待されるトークン: {type.ToString()}。実際のトークン: {Me.Tokens(Me.Index).Type.ToString()}")
        End If
    End Sub

    ''' <summary>
    ''' 現在のインデックスのトークンを返す
    ''' </summary>
    ''' <returns>Token</returns>
    Public Function GetToken() As Token
        If Me.Index < Me.Tokens.Length Then
            Return Me.Tokens(Me.Index)
        Else
            Return Nothing
        End If
    End Function

    ''' <summary>
    ''' 次のトークンを返す
    ''' </summary>
    ''' <returns>Token</returns>
    Public Function PeekToken() As Token
        If Me.Index + 1 < Me.Tokens.Length Then
            Return Me.Tokens(Me.Index + 1)
        Else
            Return Nothing
        End If
    End Function
End Class
