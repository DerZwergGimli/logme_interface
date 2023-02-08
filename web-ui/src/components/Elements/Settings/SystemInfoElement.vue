<template>
  <div>
    <div class="space-y-3">
      <h3 class="text-center dark:text-white underline pb-2">System Info</h3>
      <div>
        <system-table
          :header="system_info_table_header"
          :body="system_info_table_content"
        />
      </div>
    </div>
  </div>
</template>
<script setup lang="ts">
import { onMounted, ref } from 'vue';
import SystemTable from '../../tables/SystemTable.vue';
import { useAppStore } from '../../../stores/AppStore';

const system_info_table_header = ref(['System', 'Value']);
const system_info_table_content = ref([
  ['Target', '---'],
  ['Chip Version', '---'],
  ['CPU', '---'],
  ['Flash Size', '---'],
  ['HEAP', '10', '1'],
  ['Uptime', '---'],
  ['Time [UTC]', '---'],
]);

system_info_table_content.value[0][1] =
  useAppStore().system?.info?.target ?? '';
system_info_table_content.value[1][1] =
  useAppStore().system?.info?.major_rev.toString() +
  '.' +
  useAppStore().system?.info?.minor_rev.toString();
system_info_table_content.value[2][1] =
  useAppStore().system?.info?.cores.toString() + ' Cores';
system_info_table_content.value[3][1] =
  useAppStore().system?.info?.flash_size_MB + ' MB';
system_info_table_content.value[4][1] =
  useAppStore().system?.info?.total_heap.toString() ?? '';
system_info_table_content.value[4][2] =
  useAppStore().system?.info?.free_heap.toString() ?? '';
system_info_table_content.value[5][1] =
  convert_time(useAppStore().system?.info?.uptime_ms ?? 0) ?? 'error';
system_info_table_content.value[6][1] = useAppStore().system?.info?.time ?? '';

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
