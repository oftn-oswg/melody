# Melody

Melody is a simple program to generate music from a string of text in order to quickly share melodies.

## Install

    sudo apt-get install libsmf-dev
    make

## Run

The installer creates the `melody` binary. It is run as: `melody [-i <instrument_number>] filename.mid` where `<instrument_number>` is the general midi instrument code.

    echo 'T160 d g [g b-] a f g d . d g [g b-] a f g:3 d g [g b-] a f g d . g f:2 d+ [d+ c] d . . . d' | ./melody eboy_original.mid
    echo 'T60 [ .. 6e e-] [e e- T50 e 5b] [6d c T65 5a 4e] [a T70 5c e a] [b 4e g+ 5e] [g+ b 6c 4e] T55 [a 5e T60 6e e-] [e e- T50 e 5b] [6d c T65 5a 4e] [a T70 5c e a] [b 4e g+ 5e] T70 6c:/4 5b:/4 a:6' | ./melody furelise.mid
    echo 'T160 [6e d] 5f+ g+ [6c+ 5b] d e [b a] c+ e a:2' | ./melody granvals.mid

## Commands

<table>
	<tr>
		<th>Command</td>
		<th>Description</th>
		<th>Syntax</th>
	</tr>
	<tr>
		<td>Tempo</td>
		<td>Sets the tempo from the current position onward to <code>&lt;bpm&gt;</code> beats per minute.</td>
		<td><code>T&lt;bpm&gt;</code> or <code>t&lt;bpm&gt;</code></td>
	</tr>
	<tr>
		<td>Note</td>
		<td>Plays the note <code>&lt;class&gt;</code> from octave <code>&lt;octave&gt;</code> for <code>&lt;beats&gt;</code> beats (or 1). The octave, if not specified, will be the octave from the previous note, or 4 by default.</td>
		<td><code>&lt;octave&gt;&lt;class&gt;</code> or <code>&lt;octave&gt;&lt;class&gt;:&lt;beats&gt;</code></td>
	</tr>
	<tr>
		<td>Sub</td>
		<td>Shortens all contained notes to fit the time of one beat. Useful for things like eight notes and triplets.</td>
		<td><code>[&lt;notes&gt;]</code></td>
	</tr>
	<tr>
		<td>Rest</td>
		<td>Takes the time of one beat and makes no sound.</td>
		<td><code>.</code></td>
	</tr>
</table>

## License

Copyright © 2012 the [ΩF:∅  Working Group][1]

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

[1]: http://wg.oftn.org
