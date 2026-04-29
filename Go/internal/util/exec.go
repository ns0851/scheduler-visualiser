package util

import (
	"strings"
)

func RemoveFirstLine(text string) (string, string, error) {

	lines := strings.SplitN(text, "\n", 2)

	algo := strings.TrimSpace(lines[0])

	var input string
	if len(lines) > 1 {
		input = lines[1]
	}

	return input, algo, nil
}
