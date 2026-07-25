import { Component, input, output } from '@angular/core';

@Component({
  selector: 'app-action-switch',
  standalone: true,
  templateUrl: './action-switch.html',
  styleUrl: './action-switch.scss',
})
export class ActionSwitch {
  readonly value = input.required<boolean>();
  readonly valueChange = output<boolean>();
  readonly label = input('Target action');
  readonly trueLabel = input('On');
  readonly falseLabel = input('Off');

  protected select(nextValue: boolean): void {
    this.valueChange.emit(nextValue);
  }
}
