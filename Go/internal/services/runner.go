package service

import (
	"bytes"
	"encoding/json"
	"os/exec"
	"scheduler/Go/internal/model"
)

func RunScheduler(req model.SchedulerRequest) (model.SchedulerResponse, error) {

	var result model.SchedulerResponse

	// Convert request → JSON
	input, err := json.Marshal(req)
	if err != nil {
		return result, err
	}

	// Call C program
	cmd := exec.Command("../../../c/scheduler")
	cmd.Stdin = bytes.NewBuffer(input)

	output, err := cmd.Output()
	if err != nil {
		return result, err
	}

	// Convert JSON → struct
	err = json.Unmarshal(output, &result)
	if err != nil {
		return result, err
	}

	return result, nil
}
