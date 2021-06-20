
''' <summary>
''' ノードの種類
''' </summary>
Public Enum NodeType
    Add   ' +
    Subt  ' -
    Mul   ' *
    Div   ' /
    Num   ' 整数
End Enum

''' <summary>
''' Ast
''' </summary>
Public Class Ast

    ''' <summary>
    ''' ノードの種類
    ''' </summary>
    ''' <returns></returns>
    Public Property Type As NodeType

    Private _num As Integer
    Public ReadOnly Property Num() As Integer
        Get
            If Me.Type = NodeType.Num Then
                Return Me._num
            End If
            Throw New InvalidOperationException("整数ノードではありません")
        End Get
    End Property

    Private _lnode As Ast
    Private _rnode As Ast
    Public ReadOnly Property LNode As Ast
        Get
            Return Me._lnode
        End Get
    End Property
    Public ReadOnly Property RNode As Ast
        Get
            Return Me._rnode
        End Get
    End Property

    Private Shared Function NewNode(type As NodeType, left As Ast, right As Ast) As Ast
        Dim node As New Ast() With {
            .Type   = type,
            ._lnode = left,
            ._rnode = right
        }
        Return node
    End Function

    Private Shared Function NewNodeNum(num As Integer) As Ast
        Dim node As New Ast()
        node.Type = NodeType.Num
        node._num = num
        Return node
    End Function

    ''' <summary>
    ''' トークン列からASTを構築
    ''' </summary>
    ''' <param name="tokens">トークン列</param>
    ''' <returns>ASTの親ノード</returns>
    Public Shared Function Generate(tokens As Token()) As Ast
        Return Add(New TokenIter(tokens))
    End Function

    ''' <summary>
    ''' +, -
    ''' </summary>
    ''' <param name="tokens"></param>
    ''' <returns></returns>
    Private Shared Function Add(tokens As TokenIter) As Ast
        Dim node = Mul(tokens)
        While True
            If tokens.GetToken().Type = TokenType.Plus Then
                tokens.Consume()
                node = NewNode(NodeType.Add, node, Mul(tokens))
            ElseIf tokens.GetToken().Type = TokenType.Minus Then
                tokens.Consume()
                node = NewNode(NodeType.Subt, node, Mul(tokens))
            Else
                Return node
            End If
        End While
        Throw New NotImplementedException("Unreachable")
    End Function

    ''' <summary>
    ''' *, /
    ''' </summary>
    ''' <param name="tokens"></param>
    ''' <returns></returns>
    Private Shared Function Mul(tokens As TokenIter) As Ast
        Dim node = Term(tokens)
        While True
            If tokens.GetToken().Type = TokenType.Asterisk Then
                tokens.Consume()
                node = NewNode(NodeType.Mul, node, Term(tokens))
            ElseIf tokens.GetToken().Type = TokenType.Slash Then
                tokens.Consume()
                node = NewNode(NodeType.Div, node, Term(tokens))
            Else
                Return node
            End If
        End While
        Throw New NotImplementedException("Unreachable")
    End Function

    ''' <summary>
    ''' 整数, (Add())
    ''' </summary>
    ''' <param name="tokens"></param>
    ''' <returns></returns>
    Private Shared Function Term(tokens As TokenIter) As Ast
        Try
            ' "(" か?
            If tokens.GetToken().Type = TokenType.LParen Then
                tokens.Consume()
                Dim node = Add(tokens)
                ' ")" で終わるはず
                tokens.Expect(TokenType.RParen)
                tokens.Consume()
                Return node
            End If

            ' ()でなければ、整数のはず
            tokens.Expect(TokenType.Num)
            Dim num = tokens.GetToken().Num
            tokens.Consume()
            Return NewNodeNum(num)
        Catch ex As Exception
            MessageBox.Show(ex.Message, Application.ProductName, MessageBoxButtons.OK, MessageBoxIcon.Error)
            Throw
        End Try
    End Function

End Class
