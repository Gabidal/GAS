using "std.g"

func dsum(this, other)
(
  this : 1 = this : 1 + other : 1
  this : 0 = this : 0 + other : 0
)

type double
(
  var HI
  var LO
)

func main()
(
  new double a
  new double b
  a.dsum(b)
)