import { Component, input, output } from '@angular/core';
import { RelayTab } from '../../../core/models/relay.models';

export interface TabItem {
  id: RelayTab;
  label: string;
}

@Component({
  selector: 'app-tabs',
  standalone: true,
  templateUrl: './tabs.html',
  styleUrl: './tabs.scss',
})
export class Tabs {
  readonly tabs = input.required<TabItem[]>();
  readonly activeTab = input.required<RelayTab>();
  readonly activeTabChange = output<RelayTab>();
}
