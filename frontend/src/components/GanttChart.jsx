const GanttChart = ({ data }) => {
  if (!data || data.length === 0) return null


  const palette = ['#4f6ef7', '#10b981', '#a855f7', '#f59e0b', '#ec4899', '#14b8a6']
  const totalTime = Math.max(...data.map(d => d.end))
  const uniquePids = [...new Set(data.map(d => d.pid))].sort()
  const colorMap = Object.fromEntries(uniquePids.map((pid, i) => [pid, palette[i % palette.length]]))
  const tickValues = [...new Set([0, ...data.map(d => d.end)])]

  return (
    <div style={{ width: '100%', marginTop: 16 }}>
      <h3 style={{ fontWeight: 'bold', marginBottom: 12 }}>Gantt Chart</h3>

      <div style={{ position: 'relative', width: '100%', height: 48, background: '#f3f4f6', border: '1px solid #d1d5db', borderRadius: 8, overflow: 'hidden' }}>
        {data.map((block) => (
          <div
            key={`${block.pid}-${block.start}`}
            style={{
              position: 'absolute',
              height: '100%',
              left: `${(block.start / totalTime) * 100}%`,
              width: `${((block.end - block.start) / totalTime) * 100}%`,
              backgroundColor: colorMap[block.pid],
              borderRight: '2px solid rgba(255,255,255,0.3)',
              display: 'flex',
              alignItems: 'center',
              justifyContent: 'center',
              color: '#fff',
              fontSize: 12,
              fontWeight: 'bold',
            }}
          >
            {block.pid}
          </div>
        ))}
      </div>

      <div style={{ position: 'relative', width: '100%', height: 24, marginTop: 4 }}>
        {tickValues.map(t => (
          <span
            key={t}
            style={{
              position: 'absolute',
              left: `${(t / totalTime) * 100}%`,
              transform: t === 0 ? 'none' : 'translateX(-50%)',
              fontSize: 10,
              fontFamily: 'monospace',
              color: '#6b7280',
            }}
          >
            {t}
          </span>
        ))}
      </div>

      <div style={{ display: 'flex', gap: 12, marginTop: 10 }}>
        {uniquePids.map(pid => (
          <div key={pid} style={{ display: 'flex', alignItems: 'center', gap: 6, fontSize: 12, color: '#4b5563' }}>
            <span style={{ width: 12, height: 12, borderRadius: 3, background: colorMap[pid], display: 'inline-block' }} />
            {pid}
          </div>
        ))}
      </div>
    </div>
  )
}

export default GanttChart