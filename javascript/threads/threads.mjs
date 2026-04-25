// threads.mjs
import { Worker, isMainThread,
  workerData, parentPort } from 'node:worker_threads';

console.log('isMainThread:', isMainThread);
console.log('workerData:', workerData);
console.log('parentPort:', parentPort);

if (isMainThread) {
  const data = 'some data';
  const worker = new Worker(import.meta.filename, { workerData: data });
  worker.on('message', msg => console.log('Reply from Thread:', msg));
} else {
  const source = workerData;
  parentPort.postMessage(source.toUpperCase());

  // 在子线程中再创建子线程
  if (source !== 'final') {
    const nextData = 'final';
    const subWorker = new Worker(import.meta.filename, { workerData: nextData });
    subWorker.on('message', msg => console.log('Reply from SubThread:', msg));
  }
}

// run with `node threads.mjs`