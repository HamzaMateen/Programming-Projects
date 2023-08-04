<script setup lang="ts">
import { ref } from 'vue'

defineProps<{ msg: string, otherMessage: string }>()

const count = ref(0)

</script>

<template> 
  <div class="button">
    <h1>Count is {{ count }}</h1>

    <button type="button" @click="count++">Increment</button>
    <button type="button" @click="count <= 0 ? count : count--">Decrement</button>
  </div>

</template>

<style scoped>
.read-the-docs {
  color: #888;
}
</style>
