## Two files, building/reusing last week solution code.

### The Macro Stuff
Both do the same thing - but the `DEBUG...` file has a macro that if you change ./ toggle `#define DEBUG ` to `1`or `0` that influences how the compiler treats the flow.
- On and it enables full fat debugging - i.e. turns each usage into a Serial.print.
- Off disables and just vaporizes each use in the main code to no code


### The function() stuff
Both these then introduce the use of a function, tryign to use what we learned in lectures:

`void readSensorValues(unsigned int *delayTime, unsigned int *pwmValue) `

So we are using unsigned ints, in the main loop, passing their adderrs using **`&`**

`readSensorValues(&delayTime, &pwmValue);`

In our function we need to now take that address inbound, and get what its pointing to using **`*`**
`*delayTime = ....` and using that update its value.
