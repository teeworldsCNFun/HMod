Contents
-------
- [Prependix](#prependix)
    - [Definition style](#definition-style)
    - [Data accessing](#data-accessing)
- [IServer](#iserver)
    - [Tick](#tick)
    - [TickSpeed](#tickspeed)
    - [MaxClients](#maxclients)
    - [ClientName](#clientname)
    - [ClientClan](#clientclan)
    - [ClientCountry](#clientcountry)
    
# Prependix
## Definition style
本文档中的定义以类似cpp的方式表示，以指示类型
api正用于相应的数据或调用。

因为lua只知道数值的“number”类型，但api区分
“int”和“float”，本文档也会这样做。两者都引用“number”数据类型。
向“int”传递除整数以外的任何内容在语法上都是有效的，但会被截断
因此可能会产生意想不到的结果。

## Data accessing
api是以面向对象的方式设计的，使用lua表来提供干净的结构表示最接近本机代码结构的。使用api时，必须坚持
要使脚本正常工作，请严格遵守特定协议：

- **[data]**
    - this variable is accessed using standard lua table referencing syntax,
      which means `Table.Variable` or `Table["Variable"]`
    - gives raw access to internal native-code class attribute
    - data can be freely read from and written to at any time
    
- **[<readonly/readwrite> property]**
    - syntactically equal to data
    - important difference to data: data exchange with properties is proxied through function
      calls (getters and setters) and might possibly have side-effects. If so, they will
      be denoted in this documentation.
    - **about modifiers:**
        - `readonly`: assigning to this property will result in error
        - `readwrite`: this property can be assigned a value of one specific type

- **functions (methods)**
    - functions in this doc are identified by the syntax of variable name and brackets behind it 
    - calling high-level functions of the api that are part of a class is especially error-prone,
      as you have to use a colon (`:`) instead of period (`.`) to reference it on the table,
      so use `ClassTable:Method(Arg1, Arg2)`, *not* `ClassTable.Method(Args)` as it will result in error!
      

# IServer
## Tick
```cs
[readonly property]
int Tick
```
The current server tick

## TickSpeed
```cs
[readonly property]
int TickSpeed
```
The number of ticks per second the server is running at

## MaxClients
```cs
[readonly property]
int MaxClients
```
How many players the netserver can hold

## ClientName
```cs
string ClientName(int ClientID)
```
Returns the name of the player with ID `ClientID`

## ClientClan
```cs
string ClientClan(int ClientID)
```
Returns the clan of the player with ID `ClientID`

## ClientCountry
```cs
int ClientCountry(int ClientID)
```
Returns the country id of the player with ID `ClientID`

## ClientIngame
```cs
boolean ClientIngame(int ClientID)
```
Returns true if there is a client with the given ID on the server, otherwise false

## 
```cs

```
desc

## 
```cs

```
desc
