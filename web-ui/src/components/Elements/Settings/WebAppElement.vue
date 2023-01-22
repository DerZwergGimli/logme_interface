<template>
  <div>
    <div class="space-y-3">
      <div>
        <h3 class="text-center dark:text-white underline pb-2">WebUI-Info</h3>
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
import { ref } from 'vue';

const system_info_table_header = ref(['APP', 'Value']);
const system_info_table_content = ref([
  ['Version', APP_VERSION],
  ['Mode', import.meta.env.MODE],
  ['API-URL', APP_API_URL == '' ? '/' : APP_API_URL],
]);
</script>
