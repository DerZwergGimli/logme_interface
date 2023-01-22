<template>
  <div>
    <div class="space-y-3">
      <h3 class="text-center dark:text-white underline pb-2">System Info</h3>
      <div>
        <simple-table
          :header="system_info_table_header"
          :body="system_info_table_content"
        />
      </div>
    </div>
  </div>
</template>
<script setup lang="ts">
import SimpleTable from '../../tables/SimpleTable.vue';
import { onMounted, ref } from 'vue';

const system_info_table_header = ref(['System', 'Value']);
const system_info_table_content = ref([
  ['Target', '---'],
  ['Chip Version', '---'],
  ['CPU', '---'],
  ['Flash size', '---'],
  ['Free HEAP', '---'],
  ['Uptime', '---'],
]);

onMounted(async () => {
  fetch(APP_API_URL + '/system')
    .then(resp => resp.json())
    .then(json => {
      console.info(json);
      system_info_table_content.value[0][1] = json.info.target;
      system_info_table_content.value[1][1] =
        json.info.major_rev + '.' + json.info.minor_rev;
      system_info_table_content.value[2][1] = json.info.cores + ' Cores';
      system_info_table_content.value[3][1] = json.info.flash_size_MB + ' MB';
      system_info_table_content.value[4][1] = json.info.free_heap;
      system_info_table_content.value[5][1] =
        convert_time(json.info.uptime) ?? 'error';
    });
});

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
