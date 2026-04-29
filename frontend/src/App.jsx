import React, { useState } from 'react'
import GanttChart from './components/GanttChart'
import ProcessTable from './components/ProcessTable'

const App = () => {
  const [algo, setAlgo] = useState('fcfs')
  const [loading, setLoading] = useState(false)
  const [result, setResult] = useState(null)
  const [quantum, setQuantum] = useState(2)
  const [resetTime, setResetTime] = useState(10) // New state for MLFQ Reset
  const [processes, setProcesses] = useState([
    { id: 0, arrival: 0, burst: 5, allotment: 4 },
    { id: 1, arrival: 1, burst: 3, allotment: 4 }
  ])

  const runScheduler = async () => {
    setLoading(true)
    setResult(null)
    try {
      const res = await fetch('http://localhost:9191/', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({
          algorithm: algo,
          quantum: Number(quantum),
          reset: Number(resetTime), // Dynamic reset value
          processes: processes.map(p => ({
            id: p.id,
            arrival: p.arrival,
            burst: p.burst,
            allotment: p.allotment,
          }))
        }),
      })

      const data = await res.json()
      const timeline = data.timeline.map(t => ({
        pid: t.process,
        start: t.start,
        end: t.end,
      }))
      const metrics = Object.entries(data.metrics).map(([key, m]) => ({
        pid: key,
        completion: m.completion,
        turnaround: m.turnaround,
        response: m.response,
      }))

      setResult({ timeline, metrics })
    } catch (err) {
      alert("Backend Error: Check if server is running on 9191")
    }
    setLoading(false)
  }

  const handleChange = (id, field, value) => {
    setProcesses(prev => prev.map(p => 
      p.id === id ? { ...p, [field]: value === "" ? "" : Number(value) } : p
    ))
  }

  const addProcess = () => {
    if(processes.length >= 6) return;
    const nextId = processes.length > 0 ? Math.max(...processes.map(p => p.id)) + 1 : 0
    setProcesses([...processes, { id: nextId, arrival: 0, burst: 1, allotment: 4 }])
  }

  return (
    <div className="p-8 max-w-5xl mx-auto">
      <h1 className="text-2xl font-bold mb-6 text-gray-800">OS Scheduler</h1>
      
      <div className="grid grid-cols-1 md:grid-cols-2 gap-8 bg-white p-6 border rounded shadow-sm">
        <div>
          <label className="block text-sm font-bold mb-2 text-gray-600">Algorithm</label>
          <select 
            className="w-full border p-2 rounded mb-4 bg-gray-50 focus:ring-2 focus:ring-blue-500 outline-none"
            value={algo}
            onChange={(e) => setAlgo(e.target.value)}
          >
            <option value="fcfs">FCFS</option>
            <option value="sjfp">SJF (Preemptive)</option>
            <option value="sjf">SJF (Non-Preemptive)</option>
            <option value="rr">Round Robin</option>
            <option value="mlfq">MLFQ (Feedback Queue)</option>
          </select>

          {/* Configuration Inputs for RR and MLFQ */}
          <div className="flex gap-4 mb-6">
            {(algo === 'rr' || algo === 'mlfq') && (
              <div className="flex-1 animate-in slide-in-from-top-2 duration-200">
                <label className="block text-sm font-bold mb-2 text-blue-600">Time Quantum</label>
                <input 
                  type="number" 
                  min="1"
                  className="w-full border-2 border-blue-100 p-2 rounded focus:border-blue-400 outline-none" 
                  value={quantum} 
                  onChange={(e) => setQuantum(e.target.value)} 
                />
              </div>
            )}

            {algo === 'mlfq' && (
              <div className="flex-1 animate-in slide-in-from-top-2 duration-200">
                <label className="block text-sm font-bold mb-2 text-orange-600">Reset Time (Boost)</label>
                <input 
                  type="number" 
                  min="1"
                  className="w-full border-2 border-orange-100 p-2 rounded focus:border-orange-400 outline-none" 
                  value={resetTime} 
                  onChange={(e) => setResetTime(e.target.value)} 
                />
              </div>
            )}
          </div>

          <h3 className="font-bold mb-3 text-gray-700">Process Configuration</h3>

          {/* Column Headers */}
          <div className="flex gap-2 mb-1 px-1">
            <span className="w-8 text-[10px] font-bold text-gray-400 uppercase">ID</span>
            <span className="w-20 text-[10px] font-bold text-gray-400 uppercase">Arrival</span>
            <span className="w-20 text-[10px] font-bold text-gray-400 uppercase">Burst</span>
            {algo === 'mlfq' && (
              <span className="w-24 text-[10px] font-bold text-gray-400 uppercase">Allotted Time</span>
            )}
            <span className="w-6"></span> 
          </div>

          <div className="space-y-2">
            {processes.map(p => (
              <div key={p.id} className="flex gap-2 items-center">
                <span className="w-8 font-mono text-sm font-bold text-gray-500">P{p.id}</span>
                <input 
                  type="number" 
                  className="border p-1 w-20 text-sm rounded focus:border-blue-400 outline-none" 
                  value={p.arrival} 
                  onChange={e => handleChange(p.id, 'arrival', e.target.value)} 
                />
                <input 
                  type="number" 
                  className="border p-1 w-20 text-sm rounded focus:border-blue-400 outline-none" 
                  value={p.burst} 
                  onChange={e => handleChange(p.id, 'burst', e.target.value)} 
                />
                
                {algo === 'mlfq' && (
                  <input 
                    type="number" 
                    placeholder="Allotment"
                    className="border-2 border-orange-100 p-1 w-24 text-sm rounded focus:border-orange-400 outline-none" 
                    value={p.allotment} 
                    onChange={e => handleChange(p.id, 'allotment', e.target.value)} 
                  />
                )}

                <button 
                  onClick={() => setProcesses(processes.filter(x => x.id !== p.id))} 
                  className="text-red-300 hover:text-red-600 px-2 transition-colors"
                >
                  ✕
                </button>
              </div>
            ))}
          </div>
          
          <button 
            onClick={addProcess} 
            className="mt-4 text-blue-500 text-sm font-semibold hover:text-blue-700 flex items-center gap-1 transition-all"
          >
            <span className="text-lg">+</span> Add Process
          </button>
        </div>

        <div className="flex flex-col justify-end">
          <button 
            onClick={runScheduler}
            disabled={loading}
            className="w-full bg-slate-900 text-white py-4 rounded-lg font-bold hover:bg-black disabled:bg-gray-300 transition-all shadow-lg active:scale-[0.98]"
          >
            {loading ? 'Processing...' : 'Run Simulation'}
          </button>
        </div>
      </div>

      <div className="mt-10">
        {result ? (
          <div className="space-y-12 animate-in fade-in zoom-in-95 duration-500">
            <GanttChart data={result.timeline} />
            <ProcessTable data={result.metrics} />
          </div>
        ) : (
          <div className="py-20 border-2 border-dashed border-gray-200 rounded-xl flex flex-col items-center justify-center bg-gray-50">
             <p className="text-gray-400 font-medium italic">Configure processes and click "Run Simulation" to see the timeline.</p>
          </div>
        )}
      </div>
    </div>
  )
}

export default App