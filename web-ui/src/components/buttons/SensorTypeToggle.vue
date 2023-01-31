<script setup lang="ts">
import { ref } from 'vue';
import { Tabs, Tab } from 'flowbite-vue';

const activeTab = ref('first');
</script>

<template>
  <div class="flex flex-row items-center mb-3">
    <h3 class="basis-1/2">LogMe-Mode:</h3>
    <div class="flex w-full justify-end">
      <tabs variant="underline" v-model="activeTab" class="">
        <!-- class appends to content DIV for all tabs -->
        <tab name="first" title="Power"> </tab>
        <tab name="none" title="/" disabled></tab>
        <tab name="second" title="Heat"> </tab>
      </tabs>
    </div>
  </div>
</template>
