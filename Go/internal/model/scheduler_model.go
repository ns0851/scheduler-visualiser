package model

type Process struct {
	ID      int `json:"id"`
	Arrival int    `json:"arrival"`
	Burst   int    `json:"burst"`
	Allotment   int    `json:"allotment"`
}

type SchedulerRequest struct {
	Algorithm string    `json:"algorithm"`
	Quantum   int       `json:"quantum,omitempty"`
	Reset   int       `json:"reset,omitempty"`
	Processes []Process `json:"processes"`
}

type Interval struct {
	Process string `json:"process"`
	Start   int    `json:"start"`
	End     int    `json:"end"`
}

type Metrics struct {
	Process    string `json:"process"`
	Arrival    int `json:"arrival"`
	Burst      int `json:"burst"`
	Completion int `json:"completion"`
	Turnaround int `json:"turnaround"`
	Response    int `json:"response"`
}

type SchedulerResponse struct {
	Metrics  map[string]Metrics `json:"metrics"`
	Timeline []Interval         `json:"timeline"`
}