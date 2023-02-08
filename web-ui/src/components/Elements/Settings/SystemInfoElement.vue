<script setup lang="ts">
import { useAppStore } from '../../../stores/AppStore';
import { Progress } from 'flowbite-vue';

function convert_time(time_ms: number) {
  if (time_ms / 1000 / 60 / 60 / 24 > 1) {
    return (time_ms / 1000 / 60 / 60 / 24).toFixed(2) + ' days';
  }
  if (time_ms / 1000 / 60 / 60 > 1) {
    return (time_ms / 1000 / 60 / 60).toFixed(2) + ' h';
  }
  if (time_ms / 1000 / 60 > 1) {
    return (time_ms / 1000 / 60).toFixed(2) + ' min';
  }
  if (time_ms / 1000 > 0) {
    return (time_ms / 1000).toFixed(2) + ' s';
  }
}
</script>
<template>
  <table id="tableTABLE">
    <thead id="tableHEAD">
      <tr>
        <th>Element</th>
        <th>Value</th>
      </tr>
    </thead>
    <tbody>
      <tr>
        <td>Target</td>
        <td>{{ useAppStore().system?.info?.target }}</td>
      </tr>
      <tr>
        <td>Chip Version</td>
        <td>
          {{
            useAppStore().system?.info?.major_rev +
            '.' +
            useAppStore().system?.info?.minor_rev
          }}
        </td>
      </tr>
      <tr>
        <td>CPU</td>
        <td>{{ useAppStore().system?.info?.cores + ' Cores' }}</td>
      </tr>
      <tr>
        <td>Flash size</td>
        <td>{{ useAppStore().system?.info?.flash_size_MB + ' MB' }}</td>
      </tr>

      <tr>
        <td>Uptime</td>
        <td>{{ convert_time(useAppStore().system?.info?.uptime_ms ?? 0) }}</td>
      </tr>
      <tr>
        <td>Time [UTC]</td>
        <td>{{ useAppStore().system?.info?.time }}</td>
      </tr>
      <tr>
        <td>Heap</td>
        <td>
          <div>
            <Progress
              :labelProgress="true"
              labelPosition="outside"
              label="Free Heap"
              :progress="
                parseInt(
                  (
                    ((useAppStore().system?.info?.free_heap ?? 0) /
                      (useAppStore().system?.info?.total_heap ?? 0)) *
                    100
                  ).toFixed(0)
                )
              "
            ></Progress>
          </div>
        </td>
      </tr>
    </tbody>
  </table>
</template>
