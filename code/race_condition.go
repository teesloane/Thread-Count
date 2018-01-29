// This file demonstrates an example race condition.
// It was inspired by Katherine Cox-Buday's example in the book
// "Concurrency in Go"
package main

import (
	"fmt"
)

var num_rings int = 0

func main() {
	go incrementRings()
	fmt.Print("Number of rings to rule them all: ", num_rings)
}

func incrementRings () {
	num_rings++
}
