const ProcessTable = ({ data }) => {
  if (!data || data.length === 0) return null;

  return (
    <div className="w-full">
      <h3 className="font-semibold mb-3">Process Table</h3>
      <div className="overflow-x-auto">
        <table className="w-full border text-sm">
          <thead className="bg-gray-100">
            <tr>
              <th className="p-2 border text-left">PID</th>
              <th className="p-2 border text-center">Completion Time</th>
              <th className="p-2 border text-center">Turn Around</th>
              <th className="p-2 border text-center">Response Time</th>
            </tr>
          </thead>
          <tbody>
            {data.map((p, i) => (
              <tr key={i} className="hover:bg-gray-50">
                <td className="p-2 border font-medium">{p.pid}</td>
                <td className="p-2 border text-center">{p.completion}</td>
                <td className="p-2 border text-center">{p.turnaround}</td>
                <td className="p-2 border text-center">{p.response}</td>
              </tr>
            ))}
          </tbody>
        </table>
      </div>
    </div>
  );
};

export default ProcessTable;