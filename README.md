# Melody

Melody is a simple program to generate music from a string of text in order to quickly share melodies.

## Install

    sudo apt-get install libsmf-dev
    make

## Run

    echo 'T160 1d 1g [1g 1b-] 1a 1f 1g 1d . 1d 1g [1g 1b-] 1a 1f 1g:3 1d 1g [1g 1b-] 1a 1f 1g 1d . 1g 1f:2 1d+ [1d+ 1c] 1d . . . 1d' | ./melody eboy_original.mid

    echo 'T60 [ .. 2e 2e-] [2e 2e- T50 2e 1b] [2d 2c T65 1a 0e] [0a T70 1c 1e 1a] [1b 0e 0g+ 1e] [1g+ 1b 2c 0e] T55 [0a 1e T60 2e 2e-] [2e 2e- T50 2e 1b] [2d 2c T65 1a 0e] [0a T70 1c 1e 1a] [1b 0e 0g+ 1e] T70 2c:/4 1b:/4 1a:6' | ./melody furelise.mid && cvlc furelise.mid

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
		<td>Plays the note <code>&lt;class&gt;</code> from octave <code>&lt;octave&gt;</code> for <code>&lt;beats&gt;</code> beats (or 1)</td>
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

Copyright © 2012 by members of The ΩF:∅ Foundation

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
